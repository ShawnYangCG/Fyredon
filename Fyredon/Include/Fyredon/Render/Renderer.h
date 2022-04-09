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
	Texture* get_colour_attachment();

	// Debug
	void create_test_mesh();
	void create_test_prims();

private:
	void get_GLError(const char *file, int32_t line);

private:
	// Renderer stores pointers to each entitiy it uses, no ownership (apart from grid and ground plane)
	// Render Prims / Mesh
	std::vector<Primitive*> prims;
	GroundPlane* ground;
	Primitive* axis;

	// Render Lights (single test) rm this later...
	glm::vec3 light_pos;
	float light_strength;

	// Framebuffer
	Texture* col_tex; 
	Framebuffer* framebuffer; 

	// Renderer Draw State
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
