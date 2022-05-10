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
#include "Fyredon/Render/Assets.h"

// Ext Headers
#include "GLEW/glew.h"  // GLEW
#include "GLFW/glfw3.h" // GLFW
// GLM
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

//#define DEBUG_RENDERER
#define DEBUG_FRAMEBUFFER_CHECKS

#define GL_ERROR() get_GLError(__FILE__, __LINE__)

// =========================================== Renderer Class Implementation ===========================================
Fyredon::Renderer Fyredon::Renderer::renderer;

Fyredon::Camera* Fyredon::Renderer::active_cam;

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

	// Culling
	parms.cull_backface = false; 

	// Viewport Parameters
	bool draw_grid, draw_axis, draw_test_mesh;
	bool draw_mesh_guides;

	// Internal State
	sun_light = nullptr; 
	active_cam = nullptr;

	// Internal flags 
	flags.is_init = false; 
	flags.has_viewer_prims = false; 
}

Fyredon::Renderer::~Renderer()
{
	// Dealloc FB Resources
	if (view_framebuffers.render_framebuffer_s)  delete (view_framebuffers.render_framebuffer_s);
	if (view_framebuffers.render_framebuffer_ms) delete (view_framebuffers.render_framebuffer_ms);
	if (view_framebuffers.render_col_tex_s)      delete (view_framebuffers.render_col_tex_s);
	if (view_framebuffers.render_col_tex_ms)     delete (view_framebuffers.render_col_tex_ms);

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
	view_framebuffers.render_framebuffer_s->check();
	view_framebuffers.render_framebuffer_ms->check();
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

// Info : Active Camera is static for the Renderer singleton. 
void Fyredon::Renderer::set_active_camera(Camera* cam)
{
	active_cam = cam;
}
Fyredon::Camera* Fyredon::Renderer::get_active_camera()
{
	return active_cam; 
}

// ================== Renderer Resource Creation ==================

void Fyredon::Renderer::create_view_framebuffer_resources()
{
	// Dealloc previous View framebuffer resources
	if (flags.is_init)
	{
		if (view_framebuffers.render_framebuffer_s)  delete (view_framebuffers.render_framebuffer_s);
		if (view_framebuffers.render_framebuffer_ms) delete (view_framebuffers.render_framebuffer_ms);
		if (view_framebuffers.render_col_tex_s)      delete (view_framebuffers.render_col_tex_s);
		if (view_framebuffers.render_col_tex_ms)     delete (view_framebuffers.render_col_tex_ms);
	}
	flags.is_init = false;
	// Allocate View framebuffer resources
	view_framebuffers.render_col_tex_s      = new Texture("tex_view_colour_s", 0, width, height);
	view_framebuffers.render_col_tex_ms     = new Texture_MultiSample("tex_view_colour_ms", 0, width, height, msaa_samples);
	view_framebuffers.render_framebuffer_s  = new Framebuffer_S(view_framebuffers.render_col_tex_s);
	view_framebuffers.render_framebuffer_ms = new Framebuffer_MS(view_framebuffers.render_col_tex_ms, msaa_samples);
}

// ================== Submission and Deletion of resources to Render ==================
// Info : Public exposed function to pass primtives to internal draw array of Primitives.
//        This does not define lifetime of resource, component/ECS world does. 
std::size_t Fyredon::Renderer::submit_mesh(Mesh* mesh, const std::function<void()>& command)
{
	draw_lists.meshes.push_back(mesh);
	// Store Render command
	commands[mesh->name] = command; 
	return draw_lists.meshes.size() - 1;
}

std::size_t Fyredon::Renderer::submit_mesh(Mesh* mesh)
{
	draw_lists.meshes.push_back(mesh);
	// No Render command passes, use empty lambda. 
	commands[mesh->name] = [&]() {return;};
	return draw_lists.meshes.size() - 1; 
}

// Info : For guide meshes (like camera frustum) created externally by ECS system
std::size_t Fyredon::Renderer::submit_mesh_guide(Mesh* mesh_guide)
{
	draw_lists.guides.push_back(mesh_guide);
	// No Render command passes, use empty lambda. 
	commands[mesh_guide->name] = [&]() {return;};
	return draw_lists.guides.size() - 1; 
}

std::size_t Fyredon::Renderer::submit_point_light(Light_Point* light)
{
	point_lights.push_back(light);
	return point_lights.size() - 1; 
}

void Fyredon::Renderer::delete_mesh(std::size_t idx)
{
	draw_lists.meshes.erase(draw_lists.meshes.begin() + idx);
}

void Fyredon::Renderer::delete_mesh_guide(std::size_t idx)
{
	draw_lists.guides.erase(draw_lists.guides.begin() + idx);
}

void Fyredon::Renderer::delete_point_light(std::size_t idx)
{
	draw_lists.guides.erase(draw_lists.guides.begin() + idx);
}

void Fyredon::Renderer::resize(int width, int height)
{
	this->width  = width;
	this->height = height;
	init(ogl_wincontext, msaa_samples);
}

// ==================== Renderer Core Functions ====================
// 
// Info : Default Rendering Flags
void Fyredon::Renderer::render_setup()
{
	if (!flags.is_init) throw std::runtime_error("ERROR::Renderer Not Initalized");

	// Prim Sizes
	glPointSize(5.f);
	glLineWidth(2.5f);

	// Common
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);

	// Culling (assume CCW)
	if (parms.cull_backface) glCullFace(GL_BACK);

	// Blending 
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Fyredon::Renderer::render()
{
	// ==================== Render State ====================
	// Enforce base renderer flags
	render_setup();

	// Render from Active Camera
	render_camera(active_cam);

	// Post Processing ... not gonna happen :( 

	// Convert resulting attachments to ImGui Textures or fetch manually to GUI_Viewport...
}

// Info : Basic forward rendering pipeline from some camera, render to framebuffer.
void Fyredon::Renderer::render_camera(Camera *cam_ref)
{
	flags.frame_complete = 0; 

	// Bind View MS Framebuffer
	view_framebuffers.render_framebuffer_ms->bind_clear();
	GL_ERROR();

	// Not remaining bound for some reason ...
	view_framebuffers.render_framebuffer_ms->bind();

	// ==================== Render Game Meshes ====================
	if (draw_lists.meshes.size()) {
		
		for (Mesh *p : draw_lists.meshes) {
			// Pre Render Command
			commands[p->name](); 
			// Render Mesh
			p->render();
		}
	}
	GL_ERROR(); 

	// ==================== Render Guide Meshes ====================
	if (draw_lists.guides.size()) {
		for (Mesh* p : draw_lists.guides)
		{
			p->render();
		}
	}
	
	draw_viewer_prims();
	// ==================== Post Render ====================
	// Blind to S framebuffer
	view_framebuffers.render_framebuffer_ms->blit_to_framebuffer_s(view_framebuffers.render_framebuffer_s);
	glBindFramebuffer(GL_FRAMEBUFFER, 0); 

	flags.frame_complete = 1; 

	// ====================  Swap and Poll ====================
	// Render to glfw framebuffer for debugging only (bypass GUI texture)
#ifdef DEBUG_RENDERER
	glfwSwapBuffers(ogl_wincontext);
	glfwPollEvents();
#endif
}

// Info : Return Renderer primary colour attachement (resolved from multisampled-->standard texture)
Fyredon::Texture* Fyredon::Renderer::get_colour_attachment()
{ return view_framebuffers.render_col_tex_s; }

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
	viewer_prims.axis = new Mesh("axis", Mesh_Type::GUIDE_MESH);
	viewer_prims.axis->set_data_mesh(Fyredon::Assets::axis_verts);
	viewer_prims.axis->translate(glm::vec3(0.f, 0.01f, 0.f));
}

void Fyredon::Renderer::draw_viewer_prims()
{
	if (parms.draw_grid) viewer_prims.ground->render();

	if (parms.draw_test_mesh) viewer_prims.test_mesh->render();

	if (parms.draw_axis) {
		glLineWidth(2.5f);
		viewer_prims.axis->render();
	}

}