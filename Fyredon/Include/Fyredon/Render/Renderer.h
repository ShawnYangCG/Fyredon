// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	Renderer.h: Modern OpenGL Rendering : Niall
=============================================================================*/
#pragma once

// Info : Renderer only deals with base engine types, all ECS based classes are responsible
// for submitting their data to the renderer for drawing and managing any GUI or additonal state. 

// Std Includes
#include <string>
#include <functional>

// Project Headers
#include "Fyredon/Render/Camera.h"
#include "Fyredon/Render/Mesh.h"
#include "Fyredon/Render/Mesh_OBJ.h"
#include "Fyredon/Render/GroundPlane.h"
#include "Fyredon/Render/Light.h"


// Ext Headers
#include "glm/glm.hpp"

// Inital render res (pre viewport scaling)
#define RENDER_RES_X 768
#define RENDER_RES_Y 504

struct GLFWwindow;

namespace Fyredon {

// ================ Forward Decls ================
class Framebuffer_S;
class Framebuffer_MS;
class Texture; 
class Texture_MultiSample;
struct GLFWState;

enum Framebuffer_Attachment : uint8_t 
{
	VIEW_COLOUR = 0, VIEW_DEPTH, SHADOW_DEPTH 
};

// ================ Renderer Class ================

class Renderer
{
public:
	~Renderer();
	static Renderer& get_renderer();

	void init(GLFWwindow* ogl_context, int8_t sample_count);

	static void set_active_camera(Camera* cam);

	static Camera* get_active_camera();

	void render(); 

	// ToDo : get_framebuffer_attachemnt (ENUM : view_colour, view_depth, shadow_depth etc...)
	Texture* get_colour_attachment();

public:
	void resize(int width, int height);

	// Submit to draw lists for rendering
	std::size_t submit_mesh(Mesh* mesh, const std::function<void()> &command);

	std::size_t submit_mesh(Mesh* mesh);

	std::size_t submit_mesh_guide(Mesh* mesh_guide);

	std::size_t submit_point_light(Light_Point* light);

	// Remove from draw lists
	void delete_mesh              (std::size_t idx); 

	void delete_mesh_guide        (std::size_t idx);

	void delete_point_light       (std::size_t idx);

	// ToDo : Debug Guide Mesh creation
	// void draw_guide_line(glm::vec3 pos, float length, float width, float lifespan); 
	// void draw_guide_point(glm::vec3 pos, float size, float lifespan); 

	struct
	{
		// Sun Light (intrinsic to renderer only)
		bool enable_sun; 
		glm::vec3 sun_direction, sun_colour;
		float sun_strength; 

		// Shadow Parameters
		bool sun_shadows; 
		std::size_t shadowmap_res_sqr; 

		// Skybox Parameters : ToDo
		// Skybox Tex, Use Skybox, Skybox res...

		// Reflection Parameters : ToDo
		// cubemap reflections

		// Post Processing Parameters : ToDo
		// use bloom

		// Viewport Parameters
		bool draw_grid, draw_axis, draw_test_mesh;
		bool draw_mesh_guides;
		// std::size_t grid_size;

		// Culling
		bool cull_backface; 

	} parms;

	int32_t width  = RENDER_RES_X;
	int32_t height = RENDER_RES_Y;
	int8_t msaa_samples;

private:
	Renderer();

	// Core Rendering
	void render_setup();

	void render_camera(Camera* cam_ref);

	void draw_viewer_prims(); 

	// Resource Creation
	void create_view_framebuffer_resources(); 

	// Debug Resources Creation
	void create_test_mesh();

	void create_render_prims();

	// Debugging 
	void get_GLError(const char* file, int32_t line);

private:
	// Renderer singleton 
	static Renderer renderer; 

	// =========== Meshes To Draw ============
	struct
	{
		std::vector<Mesh*> meshes;

		std::vector<Mesh*> guides;

	} draw_lists;

	// Mesh render commands
	std::unordered_map<std::string, std::function<void()>> commands;

	struct // Internal meshes
	{
		GroundPlane* ground;
		Mesh* axis;
		Mesh_OBJ* test_mesh;
	} viewer_prims;

	// =========== Lights ============
	Light_Directional*        sun_light; 
	std::vector<Light_Point*> point_lights;

	// =========== Resources ============
	struct
	{
		Framebuffer_S*       render_framebuffer_s;
		Framebuffer_MS*      render_framebuffer_ms; 
		Texture*             render_col_tex_s;
		Texture_MultiSample* render_col_tex_ms; 

	} view_framebuffers;

	/* ToDo
	struct 
	{
	  Framebuffer_S *sun_shadow_map
	} light_framebuffers;  */

	struct
	{
		int8_t is_init, has_viewer_prims : 1; 
		int8_t frame_complete : 1; 
	} flags;

	// =========== Ext References ============
	static Camera* active_cam;
	GLFWwindow* ogl_wincontext; 
};

}
