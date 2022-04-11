// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	Renderer.h: Modern OpenGL Rendering : Niall
=============================================================================*/
#pragma once

// Std Includes
#include <string>

// Project Headers
#include "Fyredon/Render/Camera.h"
#include "Fyredon/Render/Primitive.h"
#include "Fyredon/Render/GroundPlane.h"
#include "Fyredon/Render/Mesh.h"

// Ext Headers
#include "glm/glm.hpp"

struct GLFWwindow;

namespace Fyredon {

class Framebuffer;
struct GLFWState;

class Renderer
{
public:
	Renderer(GLFWwindow* ogl_context);
	~Renderer() = default;

	// Rendering
	void render_prep();
	void render(Camera& cam_ref); // Single Tick of renderer 
	Texture* get_colour_attachment();



private:
	void get_GLError(const char *file, int32_t line);

	// Debug
	void create_test_mesh();
	void create_test_prims();

public:
	// Exposed to Engine
	void submit_primitive(Primitive* prim);

	// Public Renderer flags (to hook from GUI) 
	struct
	{
		bool *draw_grid, *draw_axis, *draw_test_prims;
		bool *ren_normals, *ren_wire;
	} flags;


private:
	// Primtives for Renderer to draw per frame 
	std::vector<Primitive*> prims;

	GroundPlane* ground;
	Primitive* axis;

	// Render Lights (single test) rm this later...
	// std::vector<Light*> lights; 
	struct
	{
		glm::vec3 position;
		float     strength;

	} dummy_light;

	// Framebuffer
	Texture* col_tex; 
	Framebuffer* framebuffer; 


	GLFWwindow* ogl_wincontext; 
};

}
