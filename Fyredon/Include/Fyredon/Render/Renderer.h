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

public:
	// Exposed to Engine
	void submit_primitive(Primitive* prim);
	void submit_light(Light* light);

	// Public Renderer flags (to hook from GUI) 
	struct
	{
		bool *draw_grid, *draw_axis, *draw_test_prims;
		bool *ren_normals, *ren_wire;
	} flags;

private:
	static Renderer renderer; 

	// Draw Lists for Renderer to draw per frame 
	std::vector<Primitive*> prims;
	
	// Renderers own prims 
	GroundPlane* ground;
	Primitive* axis;

	std::vector<Light*> lights; 
	// rm me later... need to support multiple lights first. 
	struct
	{
		glm::vec3 position;
		float     strength;

	} dummy_light;

	// Framebuffer
	Texture* col_tex; 
	Framebuffer* framebuffer; 


	GLFWwindow* ogl_wincontext; 
	bool is_init; 
};

}
