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
#include "Fyredon/Terrain/TerrainGenerator.h"

// Ext Headers
#include "glm/glm.hpp"

#define RENDER_RES_X 768
#define RENDER_RES_Y 512


struct GLFWwindow;

namespace Fyredon {

class Framebuffer;
class Light; 
struct GLFWState;

class Renderer
{
public:
	~Renderer() = default;

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
	void create_test_prims();

	void create_terrain();

public:
	// Exposed to Engine
	void submit_primitive(Primitive* prim);
	void submit_light(Light* light);
	void resize(int width, int height);

	// Public Renderer flags (to hook from GUI) 
	struct
	{
		bool *draw_grid, *draw_axis, *draw_test_prims;
		bool *ren_normals, *ren_wire;
		glm::vec3* light_pos, *test_pos; 
		float* light_str; 
	} flags;

	int width = RENDER_RES_X;
	int height = RENDER_RES_Y;

private:
	static Renderer renderer; 

	// Draw Lists for Renderer to draw per frame 
	std::vector<Primitive*> prims;
	
	// Renderers own prims 
	GroundPlane* ground;
	Mesh* test_mesh;
	Primitive* axis;


	TerrainGenerator* terrain_generator;
	bool terrain_loaded = false;


	// rm me later... need to support multiple lights first. 
	std::vector<Light*> lights; 

	// Framebuffer
	Texture* col_tex; 
	Framebuffer* framebuffer; 


	GLFWwindow* ogl_wincontext; 
	bool is_init; 

	
};

}
