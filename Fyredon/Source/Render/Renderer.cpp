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
#define DEBUG_FRAMEBUFFER_CHECKS

#define GL_ERROR() get_GLError(__FILE__, __LINE__)

// =========================================== Renderer Class Implementation ===========================================
Fyredon::Renderer Fyredon::Renderer::renderer;

Fyredon::Renderer& Fyredon::Renderer::get_renderer()
{ return renderer; }

Fyredon::Renderer::Renderer()
{
	// Parameters Init
	// Lighting - Sun
	parms.enable_sun = true, parms.sun_shadows = false;
	parms.sun_strength = 1.f;
	parms.shadowmap_res_sqr = 512; 

	// Skybox
	// ....
	// Reflections
	// ....
	// Post Processing 
	// ....

	// Viewport Parameters
	bool draw_grid, draw_axis, draw_test_mesh;
	bool draw_mesh_guides;

	// Internal State
	sun_light = nullptr; 
	cameras.active_cam = nullptr, cameras.game_cam = nullptr, cameras.scene_cam = nullptr;

	// Internal flags 
	flags.is_init = false; 
	flags.has_viewer_prims = false; 
}

Fyredon::Renderer::~Renderer()
{
	// Dealloc FB Resources
	if (primary_framebuffers.render_framebuffer_s)  delete (framebuffers.render_framebuffer_s);
	if (primary_framebuffers.render_framebuffer_ms) delete (framebuffers.render_framebuffer_ms);
	if (primary_framebuffers.render_col_tex_s)      delete (framebuffers.render_col_tex_s);
	if (primary_framebuffers.render_col_tex_ms)     delete (framebuffers.render_col_tex_ms);

	// Dealloc Viewer Prims
	if (viewer_prims.ground)    delete viewer_prims.ground;
	if (viewer_prims.axis)      delete viewer_prims.axis;
	if (viewer_prims.test_mesh) delete viewer_prims.test_mesh;
}

// Info : Get OGL Context and do intalization setup with creation of framebuffer to draw to.  
void Fyredon::Renderer::init(GLFWwindow* ogl_context, int8_t sample_count)
{
	// Assume we are on the same thread as OGL Context creation
	ogl_wincontext = ogl_context;
	msaa_samples = sample_count;

	create_view_framebuffer_resources();
	GL_ERROR();

	#ifdef DEBUG_FRAMEBUFFER_CHECKS
	framebuffers.render_framebuffer_s->check();
	framebuffers.render_framebuffer_ms->check();
	#endif 

	// Create Viewer Primitives
	if (!flags.has_viewer_prims)
	{
		create_render_prims();
		create_test_mesh();
		flags.has_viewer_prims = true;
	}
	GL_ERROR();

	flags.is_init = true; 
}

void Fyredon::Renderer::create_view_framebuffer_resources()
{
	// Dealloc previous View framebuffer resources
	if (flags.is_init)
	{
		if (primary_framebuffers.render_framebuffer_s)  delete (framebuffers.render_framebuffer_s);
		if (primary_framebuffers.render_framebuffer_ms) delete (framebuffers.render_framebuffer_ms);
		if (primary_framebuffers.render_col_tex_s)      delete (framebuffers.render_col_tex_s);
		if (primary_framebuffers.render_col_tex_ms)     delete (framebuffers.render_col_tex_ms);
	}
	flags.is_init = false;
	// Allocate View framebuffer resources
	primary_framebuffers.render_col_tex_s      = new Texture("tex_view_colour_s", 0, width, height);
	primary_framebuffers.render_col_tex_ms     = new Texture_MultiSample("tex_view_colour_ms", 0, width, height, msaa_samples);
	primary_framebuffers.render_framebuffer_s  = new Framebuffer_S(framebuffers.render_col_tex_s);
	primary_framebuffers.render_framebuffer_ms = new Framebuffer_MS(framebuffers.render_col_tex_ms, msaa_samples);
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
	init(ogl_wincontext, msaa_samples);
}

void Fyredon::Renderer::set_active_camera(Camera* active_camera)
{
	cameras.active_cam = active_camera; 
}

// Info : Default Rendering Flags
void Fyredon::Renderer::render_setup()
{
	if (!is_init) throw std::runtime_error("ERROR::Renderer Not Initalized");

	// Prim Sizes
	glPointSize(5.f);
	glLineWidth(2.5f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	// Blending 
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Fyredon::Renderer::render()
{
	render_camera(*game_cam);
}

// Info : Basic forward rendering pipeline from some camera, render to framebuffer.
void Fyredon::Renderer::render_camera(Camera *cam_ref)
{
	// ==================== Render State ====================
	// Enforce base renderer flags
	render_setup();

	// Bind Renderer MS Framebuffer
	framebuffers.render_framebuffer_ms->bind_clear();
	GL_ERROR();


	// ==================== Render Game Meshes ====================

	 // Not remaining bound for some reason ...
	framebuffers.render_framebuffer_ms->bind();

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

	// Blind to S framebuffer
	framebuffers.render_framebuffer_ms->blit_to_framebuffer_s(framebuffers.render_framebuffer_s);

	// Unbind Framebuffers
	glBindFramebuffer(GL_FRAMEBUFFER, 0); 

	// ====================  Swap and Poll ====================
	// Renderting to framebuffer, should not needed (unless debugging renderer in window context). 
#ifdef DEBUG_RENDERER
	glfwSwapBuffers(ogl_wincontext);
	glfwPollEvents();
#endif
}

// Info : Return Renderer primary colour attachement (resolved from multisampled-->standard texture)
Fyredon::Texture* Fyredon::Renderer::get_colour_attachment()
{ return framebuffers.render_col_tex_s; }

// =========================================== DEBUG CODE ===========================================

// Info : Old Error Checking, not specfic at all. Keep for backup sake only.
void Fyredon::Renderer::get_GLError(const char* file, int32_t line)
{
	GLenum err = glGetError();
	if (err != GL_NO_ERROR) 
		std::cerr << "ERROR::Renderer::GL_ERROR : " << err << " line: " << line << " file: " << file << std::endl;
}

void Fyredon::Renderer::create_test_mesh()
{
	viewer_prims.test_mesh = new Mesh_OBJ("test_pig", "..//Assets//Mesh//pighead.obj");
	viewer_prims.test_mesh->load_obj();
	viewer_prims.test_mesh->mode = Render_Mode::RENDER_MESH;
	viewer_prims.test_mesh->set_shader("..//Shaders//mesh.vert", "..//Shaders//mesh.frag");
	viewer_prims.test_mesh->translate(glm::vec3(0.f, 0.2f, 2.f));
	viewer_prims.test_mesh->scale(glm::vec3(2.f));
}

// Info : Contain these test render prims here for now.
void Fyredon::Renderer::create_render_prims()
{
	// Ground Plane
	viewer_prims.ground = new GroundPlane;
	viewer_prims.ground->set_size(4.f);
	viewer_prims.ground->set_tile(2.f);

	// Axis
	viewer_prims.axis = new Mesh("axis");
	// move to assets.h
	std::vector<Vert> verts; 
	verts.emplace_back(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f));
	verts.emplace_back(glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f));
	verts.emplace_back(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 0.f, 0.f));
	verts.emplace_back(glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 0.f, 0.f));
	verts.emplace_back(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 0.f, 0.f));
	verts.emplace_back(glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 0.f, 0.f));
	viewer_prims.axis->set_data_mesh(verts);
	viewer_prims.axis->set_shader("..//Shaders//guides.vert", "..//Shaders//guides.frag");
	viewer_prims.axis->mode = Render_Mode::RENDER_LINES;
	viewer_prims.axis->translate(glm::vec3(0.f, 0.01f, 0.f));
}

void Fyredon::Renderer::draw_viewer_prims()
{
	if (viewer_prims.test_mesh) // Update Test Mesh
	{
		viewer_prims.test_mesh->shader.setVec("camPos_world", cam_ref.position);
		viewer_prims.test_mesh->shader.setVec("lightPos_world", *flags.light_pos);
		viewer_prims.test_mesh->shader.setFloat("lightStr", *flags.light_str);
	}

	// ==================== Render Viewer Primtivies ====================
	// Draw Grid 

	if (parms.draw_grid)
	{
		viewer_prims.ground->set_cameraTransform(cam_ref.get_view_matrix(), cam_ref.get_proj_matrix());
		viewer_prims.ground->render();
	}
	if (parms.draw_axis)
	{
		glLineWidth(2.5f);

		viewer_prims.axis->set_cameraTransform(cam_ref.get_view_matrix(), cam_ref.get_proj_matrix());
		viewer_prims.axis->render();
	}
	if (parms.draw_test_mesh)
	{
		viewer_prims.test_mesh->set_cameraTransform(cam_ref.get_view_matrix(), cam_ref.get_proj_matrix());
		viewer_prims.test_mesh->render();
	}
}