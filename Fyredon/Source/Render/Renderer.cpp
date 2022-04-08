// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	Renderer.h: Modern OpenGL Rendering : Niall
=============================================================================*/
#include "Fyredon/Other/pch.h"
// Impelments
#include "Fyredon/Render/Renderer.h"

#include "Fyredon/Render/Framebuffer.h"

// Std Headers / Use PCH
//#include <iostream>
//#include <vector>
#include <thread>

// Ext Headers
// GLEW
#include "GLEW/glew.h" 
// GLFW
#include "GLFW/glfw3.h" 
// GLM
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


// =========================================== Viewer Class Implementation ===========================================

Fyredon::Renderer::Renderer(GLFWwindow* ogl_context) :
	ogl_wincontext(ogl_context)
{
	draw_prims = true; 
	draw_grid = true;
	draw_axis = true;
	ren_normals = false; 
	ren_wire = false; 

	// Temp
	light_strength = 2.0f;
	light_pos = glm::vec3(0.f, 5.f, 3.f);

	// Framebuffer Setup
	get_GLError();
	int w, h; 
	glfwGetWindowSize(ogl_context, &w, &h);
	col_tex     = new Texture("ColorAttachment", 0, w, h);
	framebuffer = new Framebuffer(col_tex);
	get_GLError();
	render_prep();

	create_test_prims();
}

// Initalize Render State (abstract these calls later)
void Fyredon::Renderer::render_prep()
{
	// Multisampling 
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
	glClearColor(0.15f, 0.15f, 0.15f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Bind Framebuffer
//	framebuffer->clear();
	framebuffer->bind(); 

	// ==================== Render Viewer Primtivies ====================
	
	//get_GLError();
	// Draw Grid 
	if (draw_grid)
	{
		ground->set_cameraTransform(cam_ref.get_view_matrix(), cam_ref.get_proj_matrix());
		ground->render();
	}

	/*

	// Draw Axis
	if (draw_axis)
	{
		axis->set_cameraTransform(cam_ref.get_view_matrix(), cam_ref.get_proj_matrix());
		glLineWidth(2.5f); // Reset for axis width.
		axis->render();
	}
	// Draw Primtivies
	get_GLError();
	if (prims.size() && draw_prims)
	{
		for (Primitive *p : prims)
		{
			// Update Uniforms
			p->shader.setVec("camPos_world", cam_ref.position);
			p->shader.setVec("lightPos_world", light_pos);
			p->shader.setFloat("lightStr",     light_strength);

			p->set_cameraTransform(cam_ref.get_view_matrix(), cam_ref.get_proj_matrix());
			p->render();
		}
	}
	get_GLError();
	*/

	// Unbind Framebuffers
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// ====================  Swap and Poll ====================
	// Renderting to framebuffer, not needed. 
	glfwSwapBuffers(ogl_wincontext);
	glfwPollEvents();
}


Fyredon::Texture* Fyredon::Renderer::get_colour_attachment()
{
	return col_tex; 
}

// =========================================== DEBUG CODE ===========================================

// Terrible Error Checking, not specfic at all. 
void Fyredon::Renderer::get_GLError()
{
	GLenum err = glGetError();
	if (err != GL_NO_ERROR) std::cerr << "ERROR::Viewer::GL_ERROR = " << err << std::endl;
}

void Fyredon::Renderer::test_mesh()
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
	// Ground Plane
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

