// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	Renderer.h: Modern OpenGL Rendering : Niall
=============================================================================*/
#pragma once

// Std Includes
#include <string>

// Project Headers
#include "Fyredon/Render/Camera.h"
#include "Fyredon/Render/Mesh.h"
#include "Fyredon/Render/Mesh_OBJ.h"
#include "Fyredon/Render/GroundPlane.h"
#include "Fyredon/Core/GameWorld.h"

// Ext Headers
#include "glm/glm.hpp"

#define RENDER_RES_X 768
#define RENDER_RES_Y 504

struct GLFWwindow;

namespace Fyredon {

class Framebuffer;
class Light; 
class Texture; 
struct GLFWState;

class Renderer
{
public:
	~Renderer();

	static Renderer& get_renderer();
	void init(GLFWwindow* ogl_context);

	// Rendering
	void render(Camera& cam_ref); // Single Tick of renderer 

	Texture* get_colour_attachment();

private:
	Renderer();

	void get_GLError(const char *file, int32_t line);

	void render_prep();

	// Debug
	void create_test_mesh();
	void create_render_prims();

public:
	// Exposed to Engine
	void submit_primitive(Mesh* prim);
	void submit_light(Light* light);
	void resize(int width, int height);

	// Public Renderer flags (to hook from GUI) 
	struct
	{
		glm::vec3 *light_pos, *test_pos; 
		bool *draw_grid,   *draw_axis, *draw_test_prims;
		bool *ren_normals, *ren_wire;
		float *light_str; 
	} flags;

	int width  = RENDER_RES_X;
	int height = RENDER_RES_Y;

private:
	static Renderer renderer; 

	// Draw Lists for Renderer to draw per frame 
	// Draw Prims
	std::vector<Mesh*> prims;
	// Draw Lights
	std::vector<Light*> lights;

	// Renderers own prims 
	GroundPlane* ground;
	Mesh*        axis;
	Mesh_OBJ*    test_mesh;
	Mesh* idx;

	// Framebuffer
	Texture* col_tex; 
	Framebuffer* framebuffer; 

	GLFWwindow* ogl_wincontext; 
	bool is_init; 
	bool has_testprims; 
};

}
