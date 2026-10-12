// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	Renderer.h: Modern OpenGL Rendering : Niall
=============================================================================*/
#pragma once

// Std Includes
#include <string>
#include <functional>

// Project Headers
#include "Fyredon/Render/Camera.h"
#include "Fyredon/Render/Mesh.h"
#include "Fyredon/Render/Mesh_OBJ.h"
#include "Fyredon/Render/GroundPlane.h"
#include "Fyredon/Core/GameWorld.h"

// Ext Headers
#include "glm/glm.hpp"

// Inital render res
#define RENDER_RES_X 768
#define RENDER_RES_Y 504

struct GLFWwindow;

namespace Fyredon {

// Forward Decls 
class Framebuffer_S;
class Framebuffer_MS;
class Light; 
class Texture; 
class Texture_MultiSample;
struct GLFWState;

// ================ Renderer Class ================

class Renderer
{
public:
	~Renderer();

	static Renderer& get_renderer();
	void init(GLFWwindow* ogl_context, int8_t sample_count);

	// Rendering
	void render(); // Single Tick of renderer 

	Texture* get_colour_attachment();

private:
	Renderer();

	void get_GLError(const char *file, int32_t line);

	void render_prep();


	void render_camera(Camera& cam_ref);

	// Debug
	void create_test_mesh();
	void create_render_prims();

public:
	// Exposed to Engine
	void submit_mesh(Mesh* mesh, const std::function<void()> &command);
	void submit_mesh(Mesh* mesh);
	void remove_mesh(Mesh* mesh);
    void submit_light(Light* light);
	void resize(int width, int height);

	// Public Renderer flags (to hook from GUI) (Rename to params)
	struct
	{
		glm::vec3 *light_pos, *test_pos; 
		bool *draw_grid,   *draw_axis, *draw_test_prims;
		bool *ren_normals, *ren_wire;
		float *light_str; 
	} flags;

	int32_t width  = RENDER_RES_X;
	int32_t height = RENDER_RES_Y;
	int8_t msaa_samples;
	struct Environment {
		int light_type = 0; // Directional, point.
		glm::vec3 light_colour = glm::vec3(1.0f, 0.97f, 0.92f);
		glm::vec3 light_direction = glm::vec3(-0.4f, -1.0f, -0.3f);
		glm::vec3 ambient_colour = glm::vec3(0.8f, 0.87f, 1.0f);
		float ambient_strength = 0.25f;
		float specular_strength = 0.08f;
		float roughness = 0.7f;
		int background = 1; // Solid, gradient sky.
		glm::vec3 solid = glm::vec3(0.15f);
		glm::vec3 sky_top = glm::vec3(0.16f, 0.32f, 0.55f);
		glm::vec3 sky_horizon = glm::vec3(0.64f, 0.73f, 0.83f);
		glm::vec3 sky_bottom = glm::vec3(0.19f, 0.22f, 0.28f);
	} environment;

	// Camera references 
	Camera* game_cam;
	// Camera* scene_cam; 

private:
	static Renderer renderer;
	Shader* background_shader = nullptr;
	GLuint background_vao = 0; 

	// ===== Draw Lists to render per frame =====
	std::vector<Mesh*>  meshes;
	std::vector<Light*> lights;

	std::unordered_map<std::string, std::function<void()>> commands;

	struct
	{
		GroundPlane* ground;
		Mesh* axis;
		Mesh_OBJ* test_mesh;
	} viewer_prims;

	struct
	{
		Framebuffer_S*       render_framebuffer_s;
		Framebuffer_MS*      render_framebuffer_ms; 
		Texture*             render_col_tex_s;
		Texture_MultiSample* render_col_tex_ms; 
	} framebuffers;

	GLFWwindow* ogl_wincontext; 
	bool is_init; 
	bool has_testprims; 
};

}
