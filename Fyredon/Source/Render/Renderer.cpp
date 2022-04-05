// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	Renderer.h: Modern OpenGL Rendering : Niall
=============================================================================*/
#include "Fyredon/Other/pch.h"
// Impelments
#include "Fyredon/Render/Renderer.h"

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
	ogl_wincontext(ogl_wincontext)
{
	tick_c = 0;
	draw_prims = true; 
	draw_grid = true;
	draw_axis = true;
	light_strength = 2.0f; 
	light_pos = glm::vec3(0.f, 5.f, 3.f);
	ren_normals = false; 
	ren_wire = false; 
}

// Initalize Render State
void Fyredon::Renderer::render_prep()
{
	// Multisampling 
	glEnable(GL_MULTISAMPLE);

	// Prim Sizes
	glPointSize(5.f);
	glLineWidth(2.5f);

	// Blending and Depth. 
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
	axis->set_shader("../../../Shaders/guides.vert", "../../../Shaders/guides.frag");
	axis->mode = Render_Mode::RENDER_LINES;
}

// Info : Basic forward rendering pipeline, render to framebuffer.
void Fyredon::Renderer::render(Camera &cam_ref)
{
	// ==================== Render State ====================
	glClearColor(0.15f, 0.15f, 0.15f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// ==================== Render Viewer Primtivies ====================
	get_GLError();
	// Draw Grid 
	if (draw_grid)
	{
		ground->set_cameraTransform(cam_ref.get_view_matrix(), cam_ref.get_proj_matrix());
		ground->render();
	}

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

	// ====================  Swap and Poll ====================
	get_GLError();

	//glfwSwapBuffers(window);
	//glfwPollEvents();
}

void Fyredon::Renderer::query_drawState()
{
	if (glfwGetKey(ogl_wincontext, GLFW_KEY_G) == GLFW_PRESS)
	{
		draw_grid = !draw_grid;
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	if (glfwGetKey(ogl_wincontext, GLFW_KEY_H) == GLFW_PRESS)
	{
		draw_axis = !draw_axis;
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

// Terrible Error Checking, not specfic at all. 
void Fyredon::Renderer::get_GLError()
{
	GLenum err = glGetError();
	if (err != GL_NO_ERROR) std::cerr << "ERROR::Viewer::GL_ERROR = " << err << std::endl;
}

// =========================================== DEBUG CODE ===========================================

void Fyredon::Renderer::test_mesh()
{
	Mesh *pig = new Mesh("test_pig", "../../assets/mesh/pighead.obj");
	pig->load_obj(false);
	pig->set_shader("../../shaders/mesh.vert", "../../shaders/mesh.frag");
	pig->set_colour(glm::vec3(1.f, 0.f, 0.f));
	pig->translate(glm::vec3(0.f, 0.2f, 2.f));
	pig->scale(glm::vec3(2.f));
	pig->mode = Render_Mode::RENDER_MESH;
	prims.push_back(pig);
}

