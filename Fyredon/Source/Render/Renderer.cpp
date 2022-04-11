// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	Renderer.h: Modern OpenGL Rendering : Niall
=============================================================================*/
#include "Fyredon/Other/pch.h"
// Impelments
#include "Fyredon/Render/Renderer.h"

// Project Headers
#include "Fyredon/Render/Framebuffer.h"
#include "Fyredon/Render/Light.h"

// Ext Headers
#include "GLEW/glew.h" // GLEW
#include "GLFW/glfw3.h" // GLFW
// GLM
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

//#define DEBUG_RENDERER
#define GL_ERROR() get_GLError(__FILE__, __LINE__)


// =========================================== Renderer Class Implementation ===========================================
Fyredon::Renderer Fyredon::Renderer::renderer;

Fyredon::Renderer& Fyredon::Renderer::get_renderer()
{ return renderer; }

Fyredon::Renderer::Renderer()
{
	// These will point to GUI Booleans 
	flags.draw_grid       = nullptr;
	flags.draw_axis       = nullptr;
	flags.draw_test_prims = nullptr;
	flags.ren_normals     = nullptr; 
	flags.ren_wire        = nullptr; 

	//for (std::size_t i = 0; i < (sizeof(flags) / sizeof(bool*)); ++i) {bool* b = ((bool*)&flags) + i; b = nullptr; }

	// Replace with Light Array
	dummy_light.strength = 2.0f;
	dummy_light.position = glm::vec3(0.f, 5.f, 3.f);

	is_init = false; 
}

// Info : Get OGL Context and do inital setup. 
void Fyredon::Renderer::init(GLFWwindow* ogl_context)
{
	ogl_wincontext = ogl_context;

	// Framebuffer Setup (Should not be tied to Window Size in Future)
	GL_ERROR();
	int w, h;
	glfwGetWindowSize(ogl_context, &w, &h);
	col_tex     = new Texture("ColorAttachment", 0, w, h);
	framebuffer = new Framebuffer(col_tex);
	GL_ERROR();

	// Test Primitives
	create_test_prims();
	create_test_mesh();
	GL_ERROR();

	is_init = true; 
}

// Info : Public exposed function to pass primtives to internal draw array of Primitives.
void Fyredon::Renderer::submit_primitive(Primitive* prim)
{
	prims.push_back(std::move(prim));
}

void Fyredon::Renderer::submit_light(Light* light)
{
	lights.push_back(std::move(light));
}

// Initalize Render State (abstract these calls later)
void Fyredon::Renderer::render_prep()
{
	// Check Renderer is ready
	if (!is_init) throw std::runtime_error("Renderer Not Initalized");

	// Multisampling (Disabled for iteration 1, needs multi-sampled texture + fbo setup)
	//glEnable(GL_MULTISAMPLE);

	// Prim Sizes
	glPointSize(5.f);
	glLineWidth(2.5f);

	// Blending and Depth. 
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

// Info : Basic forward rendering pipeline, render to framebuffer.
void Fyredon::Renderer::render(Camera &cam_ref)
{
	// ==================== Render State ====================
	// Enforce base renderer flags
	render_prep();

	// Bind Renderer Framebuffer
	framebuffer->bind_clear(); 
	GL_ERROR();

	// ==================== Render Viewer Primtivies ====================
	// Draw Grid 
	if (*flags.draw_grid)
	{
		ground->set_cameraTransform(cam_ref.get_view_matrix(), cam_ref.get_proj_matrix());
		ground->render();
	}

	// Draw Axis
	if (*flags.draw_axis)
	{
		axis->set_cameraTransform(cam_ref.get_view_matrix(), cam_ref.get_proj_matrix());
		glLineWidth(2.5f); // Reset for axis width.
		axis->render();
	}
	// Draw Primtivies
	GL_ERROR();

	framebuffer->bind(); // Not remaining bound for some reason ...

	if (prims.size() && (*flags.draw_test_prims))
	{
		for (Primitive *p : prims)
		{
			// Update Uniforms
			p->shader.setVec("camPos_world",   cam_ref.position);
			p->shader.setVec("lightPos_world", dummy_light.position);
			p->shader.setFloat("lightStr",     dummy_light.strength);

			p->set_cameraTransform(cam_ref.get_view_matrix(), cam_ref.get_proj_matrix());
			p->render();
		}
	}
	GL_ERROR(); 

	// Unbind Framebuffers
	glBindFramebuffer(GL_FRAMEBUFFER, 0); 

	// ====================  Swap and Poll ====================
	// Renderting to framebuffer, should not needed (unless debugging renderer in window context). 
#ifdef DEBUG_RENDERER
	glfwSwapBuffers(ogl_wincontext);
	glfwPollEvents();
#endif
}


Fyredon::Texture* Fyredon::Renderer::get_colour_attachment()
{ return col_tex; }

// =========================================== DEBUG CODE ===========================================

// Info : Terrible Error Checking, not specfic at all. 
void Fyredon::Renderer::get_GLError(const char* file, int32_t line)
{
	GLenum err = glGetError();
	if (err != GL_NO_ERROR) 
		std::cerr << "ERROR::Renderer::GL_ERROR : " << err << " line: " << line << " file: " << file << std::endl;
}

void Fyredon::Renderer::create_test_mesh()
{
	Mesh *pig = new Mesh("test_pig", "..//Assets//Mesh//pighead.obj");
	pig->load_obj(false);
	pig->set_shader("..//Shaders//mesh.vert", "..//Shaders//mesh.frag");
	pig->set_colour(glm::vec3(1.f, 0.f, 0.f));
	pig->translate(glm::vec3(0.f, 0.2f, 2.f));
	pig->scale(glm::vec3(2.f));
	pig->mode = Render_Mode::RENDER_MESH;
	prims.push_back(pig);
}

// Info : Contain these test render prims here for now.
void Fyredon::Renderer::create_test_prims()
{
	// Ground Plane (1281 debug)
	ground = new GroundPlane;
	ground->set_size(4.f);
	ground->set_tile(2.f);

	// Axis
	axis = new Primitive("axis");
	float data[66] =
	{
		0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f,
		1.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f,
		0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f,
		0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f,
	};
	axis->set_data_mesh(data, 6);
	axis->scale(glm::vec3(1.f));
	axis->translate(glm::vec3(0.f, 0.01f, 0.f));
	axis->set_shader("..//Shaders//guides.vert", "..//Shaders//guides.frag");
	axis->mode = Render_Mode::RENDER_LINES; 
}

