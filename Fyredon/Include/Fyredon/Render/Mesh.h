// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	Mesh.h: Key class used for handling all mesh data : Niall
=============================================================================*/
#pragma once
 
// Std Headers
#include <string>
#include <vector>
#include <functional>

// GLM
#include "glm/glm.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"

// Project Headers
#include "Fyredon/Render/Shader.h"

/* 
*  Info: Base Mesh object for rendering within the viewer app.
*  Mesh format: (P(x,y,z), N(x,y,z), C(r,g,b), UV(u,v,w)). 
*  User defines mesh as tri-soup format, using indexing internally to opitmize memory usage. 
*/ 
	
namespace Fyredon {

class TextureImage;

// 48 Byte (16 Byte Aligned) Vertex 
struct Vert
{
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec3 col;
	glm::vec3 uv;

	// To allow for emplace_back()
	Vert(const glm::vec3& P, const glm::vec3& N, const glm::vec3& C, glm::vec3& U)
		: pos(P), normal(N), col(C), uv(U) {}
	// Allow for float based data
	Vert(float P_x, float P_y, float P_z, float N_x, float N_y, float N_z,
		float C_r, float C_g, float C_b, float T_u, float T_v, float T_w)
		: pos(P_x, P_y, P_z), normal(N_x, N_y, N_z), col(C_r, C_g, C_b), uv(T_u, T_v, T_w) {}

	// Equality for hashing
	bool operator== (const Vert& b) const {
		return (pos == b.pos && normal == b.normal && col == b.col && uv == b.uv);
	}
	Vert() = default; 
};

// Hash Functor for Vertex
struct VertHash 
{
	std::size_t operator() (const Vert& key) const
	{
		return (v_hash(key.pos) ^ ( v_hash(key.normal) << 1) >> 1) 
			^ (v_hash(key.col) ^ (v_hash(key.uv) << 1) >> 1);
	}
	std::hash<glm::vec3> v_hash; 
};

// ======================== Mesh Class ========================
enum Render_Mode { RENDER_MESH = 0, RENDER_LINES, RENDER_POINTS };

class Mesh
{
public:
	explicit Mesh(const char *Name);
	Mesh() = delete;

	virtual ~Mesh();

	Mesh(const Mesh& copy); 

	// Virtual Methods 
	virtual void render();

	virtual void debug()       const;

	virtual bool check_state() const;

	// Setters
	void set_data_mesh(const std::vector<Vert>& data);

	void set_colour(const glm::vec3& col);

	void set_cameraTransform(const glm::mat4x4& view, const glm::mat4x4& persp);

	void set_shader(const char* vert_path, const char* frag_path);

	// Update
	void update_data_mesh(const std::vector<Vert>& data);

	// Single vertex attribute only
	void update_data_position(const std::vector<glm::vec3> &pos_data);

	void update_data_colour  (const std::vector<glm::vec3> &col_data);

	// Mesh now supports textures
	void load_texture(const char* filepath, GLuint unit = 0);

	// Model Transforms
	void scale       (const glm::vec3 &scale);
	void translate   (const glm::vec3 &offs);
	void rotate      (float d_ang, const glm::vec3 &axis);
	void local_rotate(float x_ang, float y_ang, float z_ang);

	// Debug
	float get_vertindex_ratio();

private:
	virtual void create_vertex_buffers(const std::vector<float>& serial_verts);

	virtual void create_index_buffer(const std::vector<GLuint>& indices);

public:
	// ------- Vertex Data -------
	std::vector<Vert>   vert_data; 
	std::vector<GLuint> inds_data;
	std::size_t vert_count, index_count;

	// ------- GL Resources -------
	GLuint VBO, VAO, EBO; 
	glm::mat4 model;
	Shader shader;

	// ------- State -------
	Render_Mode mode;
	bool wireframe;
	// Required state for rendering
	struct
	{
		uint8_t vert_data_set : 1;
		uint8_t inds_data_set : 1;
		uint8_t buffers_set   : 1;
		uint8_t shader_set    : 1;
		uint8_t camera_set    : 1;
		uint8_t texture_set   : 1;
	} flags;
	// Texture State
	bool use_tex;
	TextureImage* tex;

	std::string name;
};

}