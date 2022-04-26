// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	Renderer.h: Modern OpenGL Rendering : Niall
=============================================================================*/
#include "Fyredon/Other/pch.h"
// Impelments
#include "Fyredon/Render/Renderer.h"

// Project Headers
#include "Fyredon/Render/Texture.h"
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
	// These will point to Renderer GUI Booleans 
	flags.draw_grid       = nullptr;
	flags.draw_axis       = nullptr;
	flags.draw_test_prims = nullptr;
	flags.ren_normals     = nullptr; 
	flags.ren_wire        = nullptr; 

	// Replace with Light Array
	flags.light_str = nullptr; 
	flags.light_pos = nullptr; 
	flags.test_pos  = nullptr;

	is_init = false; 
	has_testprims = false; 
}

Fyredon::Renderer::~Renderer()
{
	// Dealloc FB Resources
	delete col_tex; 
	delete framebuffer; 

	// Dealloc Test Prims
	delete ground;
	delete test_mesh;
	delete axis; 
}

// Info : Get OGL Context and do intalization setup with creation of framebuffer to draw to.  
void Fyredon::Renderer::init(GLFWwindow* ogl_context)
{
	// Assume we are on the same thread as OGL Context creation
	ogl_wincontext = ogl_context;

	// Allocate Texture and Framebuffer
	if (col_tex)     delete col_tex; 
	if (framebuffer) delete framebuffer; 
	col_tex     = new Texture("ColorAttachment", 0, width, height);
	framebuffer = new Framebuffer(col_tex);
	GL_ERROR();

	// Create Test Primitives
	if (!has_testprims)
	{
		create_render_prims();
		create_test_mesh();
		has_testprims = true; 
	}
	GL_ERROR();

	is_init = true; 
}

// Info : Public exposed function to pass primtives to internal draw array of Primitives.
void Fyredon::Renderer::submit_mesh(Mesh* mesh, const std::function<void()>& command)
{
	meshes.push_back(std::move(mesh));
	// Store Render command
	commands[mesh->name] = command; 
}

void Fyredon::Renderer::submit_mesh(Mesh* mesh)
{
	meshes.push_back(std::move(mesh));
	// No Render command passes, use empty lambda. 
	commands[mesh->name] = [&]() {return;};
}

void Fyredon::Renderer::submit_light(Light* light)
{
	lights.push_back(std::move(light));
}

void Fyredon::Renderer::resize(int width, int height)
{
	this->width  = width;
	this->height = height;
	init(ogl_wincontext);
}

// Initalize Render State (abstract these calls later)
void Fyredon::Renderer::render_prep()
{
	if (!is_init) throw std::runtime_error("ERROR::Renderer Not Initalized");

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

	if(test_mesh) // Update Test Mesh
	{
		test_mesh->shader.setVec("camPos_world", cam_ref.position);
		test_mesh->shader.setVec("lightPos_world", *flags.light_pos);
		test_mesh->shader.setFloat("lightStr", *flags.light_str);
	} 

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

	if (*flags.draw_test_prims)
	{
		test_mesh->set_cameraTransform(cam_ref.get_view_matrix(), cam_ref.get_proj_matrix());
		test_mesh->render();
	} 

	// ==================== Render Game Meshes ====================
	//GL_ERROR();

	framebuffer->bind(); // Not remaining bound for some reason ...

	if (meshes.size())
	{
		for (Mesh *p : meshes)
		{
			// Set Shared Uniforms
			p->shader.setVec("camPos_world",   cam_ref.position);
			p->shader.setVec("lightPos_world", *flags.light_pos);
			p->shader.setFloat("lightStr",     *flags.light_str);

			// Execute Render Command (to set pre-draw state of mesh)
			commands[p->name](); 

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
	test_mesh = new Mesh_OBJ("test_pig", "..//Assets//Mesh//pighead.obj");
	test_mesh->load_obj();
	test_mesh->set_shader("..//Shaders//mesh.vert", "..//Shaders//mesh.frag");
	test_mesh->translate(glm::vec3(0.f, 0.2f, 2.f));
	test_mesh->scale(glm::vec3(2.f));
	test_mesh->mode = Render_Mode::RENDER_MESH;
}

// Info : Contain these test render prims here for now.
void Fyredon::Renderer::create_render_prims()
{
	// Ground Plane (1281 debug)
	ground = new GroundPlane;
	ground->set_size(4.f);
	ground->set_tile(2.f); 

	// Axis
	axis = new Mesh("axis");
	std::vector<Vert> verts; 
	verts.emplace_back(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f));
	verts.emplace_back(glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f));
	verts.emplace_back(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 0.f, 0.f));
	verts.emplace_back(glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 0.f, 0.f));
	verts.emplace_back(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 0.f, 0.f));
	verts.emplace_back(glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 0.f, 0.f));
	axis->set_data_mesh(verts);
	axis->translate(glm::vec3(0.f, 0.01f, 0.f));
	axis->set_shader("..//Shaders//guides.vert", "..//Shaders//guides.frag");
	axis->mode = Render_Mode::RENDER_LINES;  
}

