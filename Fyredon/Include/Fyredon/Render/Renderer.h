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

// ForwardDecl
class Framebuffer; 
struct GLFWwindow;
struct GLFWState;

// Typedef 
//using byte = unsigned char;  // use std17 Byte 


namespace Fyredon
{
class Renderer
{
public:
	Renderer() = delete;
	Renderer();
	~Renderer();

	// Rendering
	void render_prep();
	void render(); // Single Tick of renderer 


	// Per Tick Operations
	void update_window();
	void update_camera();

	// State Query
	void query_drawState();
	bool esc_pressed();
	void get_dt();

	// Debug
	void test_mesh();

private:
	void get_GLError();

private:

	// Render Prims / Mesh
	std::vector<Primitive*> prims;
	Ground* ground;
	Primitive* axis;

	// Render Lights (single test) 
	glm::vec3 light_pos;
	float light_strength;
	bool ren_normals;
	bool ren_wire;

	// Render State
	bool draw_grid, draw_axis, draw_prims; 

	// Viewer Intrinsics
	std::size_t tick_c; 
	float dt;
	float cur_t, prev_t;
};

}
