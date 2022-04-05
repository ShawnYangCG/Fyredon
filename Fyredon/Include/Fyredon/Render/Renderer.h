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
	Renderer(GLFWwindow *ogl_context);
	~Renderer() = default; 

	// Rendering
	void render_prep();
	void render(Camera& cam_ref); // Single Tick of renderer 

	// Per Tick Operations
	void create_test_prims();

	// State Query
	void query_drawState();

	// Debug
	void test_mesh();

private:
	void get_GLError();

private:
	// Renderer stores pointers to each entitiy it uses, no ownership. 
	// 
	// Render Prims / Mesh
	std::vector<Primitive*> prims;
	GroundPlane* ground;
	Primitive* axis;

	// Render Lights (single test) 
	glm::vec3 light_pos;
	float light_strength;


	// Render State
	bool draw_grid, draw_axis, draw_prims; 
	bool ren_normals, ren_wire;

	/* More optimal flags
	struct 
	{
		uint8_t draw_grid : 1, draw_axis : 1, draw_prims : 1;
		uint8_t ren_normals : 1, ren_wire : 1;
	} draw_state; */

	// Passed from editor. 
	GLFWwindow* ogl_wincontext; 
};

}
