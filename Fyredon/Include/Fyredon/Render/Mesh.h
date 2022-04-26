// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	Mesh.h: Base primtive class for rendering meshes : Niall
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

/* Info : Base Mesh object for rendering within the viewer app.
*  Supports both indexed and non-indexed (Tri-Soup) meshes.
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
enum Render_Mode
{
	RENDER_MESH = 0,
	RENDER_LINES,
	RENDER_POINTS
};

class Mesh
{
public:
	Mesh(const char *Name, bool Indexed);
	Mesh() = delete;
	virtual ~Mesh();

	// Virtual Methods 
	virtual void render();

	virtual void debug()       const;

	virtual bool check_state() const;

	// Setters
	void set_data_mesh(const std::vector<Vert> &data);
	void set_data_mesh(const std::vector<Vert>& data, bool indexed);


	void set_colour(const glm::vec3& col);

	void set_cameraTransform(const glm::mat4x4& view, const glm::mat4x4& persp);

	void set_shader(const char* vert_path, const char* frag_path);

	// Update single vertex attribute only
	void update_data_position(const std::vector<glm::vec3> &pos_data);

	void update_data_colour  (const std::vector<glm::vec3> &col_data);

	// Mesh now supports textures
	void load_texture(const char* filepath, GLuint unit = 0);

	// Model Transforms
	void scale       (const glm::vec3 &scale);
	void translate   (const glm::vec3 &offs);
	void rotate      (float d_ang, const glm::vec3 &axis);
	void local_rotate(float x_ang, float y_ang, float z_ang);

private:
	virtual void create_vertex_buffers(const std::vector<float>& serial_verts);

	virtual void create_index_buffer();

	void set_data_indices(const std::vector<GLuint>& indices);

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
	bool indexed, wireframe;
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