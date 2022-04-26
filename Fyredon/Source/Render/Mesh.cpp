// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	Mesh.cpp: Base primtive class for rendering meshes : Niall
=============================================================================*/
#include "Fyredon/Other/pch.h"
// Implements 
#include "Fyredon/Render/Mesh.h"

// Project Headers
#include "Fyredon/Render/Texture.h"

// Std Headers
#include <cassert>

// Ext Headers 
#include "GLEW/glew.h" 
#include "GLFW/glfw3.h" 
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#pragma warning( disable : 26495) 
#pragma warning( disable : 26812) 

// ================================== Mesh Class Implementation ==================================

Fyredon::Mesh::Mesh(const char *Name, bool Indexed)
	: name(Name), indexed(Indexed), vert_count(0), mode(Render_Mode::RENDER_MESH), tex(nullptr), use_tex(false), wireframe(false)
{
	flags.buffers_set   = 0;
	flags.vert_data_set = 0; 
	flags.inds_data_set = 0;
	flags.shader_set    = 0;
	flags.camera_set    = 0;
	flags.texture_set   = 0;

	model = glm::mat4(1.f);
}

Fyredon::Mesh::~Mesh()
{
	if (!flags.buffers_set) return; 
	if (VAO) glDeleteVertexArrays(1, &VAO);
	if (VBO) glDeleteBuffers(1, &VBO);
	if (EBO) glDeleteBuffers(1, &EBO);

	if (tex) delete tex;
}

// ============= Rendering =============

void Fyredon::Mesh::render()
{
	// Check for state to render
	if (!check_state())
	{
		std::cerr << "ERROR::Mesh::" << name << "::Render called, with incorrectly set state." << std::endl;
		std::terminate();
	}

	// Bind Mesh State
	shader.use();

	// Activate and Bind Texture
	if (use_tex)
	{
		tex->activate();
		tex->bind();
	}

	// Update Modfied Uniforms
	shader.setMat4("model", model);

	// Render in set mode
	glBindVertexArray(VAO);
	if (indexed)  // ---- Indexed Based Drawing of Data
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		switch (mode)
		{
			case (RENDER_POINTS):
			{
				if (wireframe) {
					glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
					glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, 0);
					glPolygonMode(GL_FRONT_AND_BACK, GL_TRIANGLES); // Restore state
					break; 
				}

				glDrawElements(GL_POINTS, index_count, GL_UNSIGNED_INT, 0);
				break;
			}
			case (RENDER_LINES):
			{
				if (wireframe) {
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);
					glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, 0);
					glPolygonMode(GL_FRONT_AND_BACK, GL_TRIANGLES);
					break;
				}

				glDrawElements(GL_LINES, index_count, GL_UNSIGNED_INT, 0);
				break;
			}
			case (RENDER_MESH):
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glDrawElements(GL_TRIANGLES, inds_data.size(), GL_UNSIGNED_INT, 0);
				break;
			}
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	else  // ---- TriSoup Based Drawing of Data
	{
		switch (mode)
		{
			case (RENDER_POINTS):
			{
				if (wireframe) {
					glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
					glDrawArrays(GL_TRIANGLES, 0, vert_count);
					glPolygonMode(GL_FRONT_AND_BACK, GL_TRIANGLES);
					break;
				}
				glDrawArrays(GL_POINTS, 0, vert_count);
				break;
			}
			case (RENDER_LINES):
			{
				if (wireframe) {
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);
					glDrawArrays(GL_TRIANGLES, 0, vert_count);
					glPolygonMode(GL_FRONT_AND_BACK, GL_TRIANGLES);
					break; 
				}
				glDrawArrays(GL_LINES, 0, vert_count);
				break;
			}
			case (RENDER_MESH):
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glDrawArrays(GL_TRIANGLES, 0, vert_count);
				break;
			}
		}
	}

	// Clear State
	glUseProgram(0);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
} 

// ===================== Mesh Data Setters =====================
// 
/* ====== = Info : Mesh Data  =======
// Mesh Data Assumptions 12 * 4 byte floats. 
// Stride :  0         3         6         9
// Data   : (x,y,z) | (x,y,z) | (r,g,b) | (u,v,w) */

// Non Indexed Version 
void Fyredon::Mesh::set_data_mesh(const std::vector<Vert>& data)
{
	// Copy/Replace Vertex Mesh Data
	vert_data = data;
	vert_count = data.size();

	// Serailize into float array
	std::vector<float> serial_data;
	for (std::size_t v = 0; v < vert_count; ++v)
	{
		const Vert& vert = data[v];
		// Position 
		serial_data.emplace_back(vert.pos.x), serial_data.emplace_back(vert.pos.y), serial_data.emplace_back(vert.pos.z);
		// Normal
		serial_data.emplace_back(vert.normal.x), serial_data.emplace_back(vert.normal.y), serial_data.emplace_back(vert.normal.z);
		// Colour
		serial_data.emplace_back(vert.col.r), serial_data.emplace_back(vert.col.g), serial_data.emplace_back(vert.col.b);
		// UV
		serial_data.emplace_back(vert.uv.x), serial_data.emplace_back(vert.uv.y), serial_data.emplace_back(vert.uv.z);
	}
	flags.vert_data_set = 1;

	// Setup Buffers
	create_vertex_buffers(serial_data);
	return;
}

void Fyredon::Mesh::set_data_mesh(const std::vector<Vert> &data, bool indexed)
{
	// rm duplicates of all same vert attributes (linear search or map based hash ideally)
	// replace with indices of orginal/existing vert. 
	// Only add to new array if unique.

	std::vector<Vert> unqiue_verts; // Probs also write directly to vert array oppose to local copy
	//std::vector<GLuint> inds; 

	for (std::size_t v = 0; v < data.size(); ++v)
	{
		const Vert& vert_i = data[v];
		bool is_dupe = false; 
		std::size_t idx = -1; 

		// Search unique_verts to see if vertex exists within it already, where all attributes match.  
		// Find first dupe only. 
		for (std::size_t uv = 0; uv < unqiue_verts.size(); ++uv)
		{
			const Vert &vert_j = unqiue_verts[uv];
			if (vert_i == vert_j) {
				//std::cout << "Duplicate Found Vertex : " << v << " With Vertex : " << uv << "\n";
				is_dupe |= true;
				idx = uv; 
				break; 
			}
		}

		if (is_dupe) // Store idx to existing vert
		{
			inds_data.push_back(idx);
		}
		else // Doesn't already exist in unique verts, so append it and store idx. 
		{
			unqiue_verts.push_back(vert_i);
			inds_data.push_back(unqiue_verts.size() - 1);
		}
	}

	// Copy/Replace Vertex Mesh Data
	vert_data = unqiue_verts;
	vert_count = unqiue_verts.size();

	// Serailize into float array
	std::vector<float> serial_data; 
	for (std::size_t v = 0; v < vert_count; ++v)
	{
		const Vert &vert = unqiue_verts[v];
		// Position 
		serial_data.emplace_back(vert.pos.x), serial_data.emplace_back(vert.pos.y), serial_data.emplace_back(vert.pos.z);
		// Normal
		serial_data.emplace_back(vert.normal.x), serial_data.emplace_back(vert.normal.y), serial_data.emplace_back(vert.normal.z);
		// Colour
		serial_data.emplace_back(vert.col.r), serial_data.emplace_back(vert.col.g), serial_data.emplace_back(vert.col.b);
		// UV
		serial_data.emplace_back(vert.uv.x), serial_data.emplace_back(vert.uv.y), serial_data.emplace_back(vert.uv.z);
	}
	flags.vert_data_set = 1;
	flags.inds_data_set = 1;

	// Setup Buffers
	create_vertex_buffers(serial_data);
	create_index_buffer();
}

void Fyredon::Mesh::set_data_indices(const std::vector<GLuint>& data)
{
	inds_data = data;
	index_count = data.size();

	flags.inds_data_set = 1;

	// Setup Index Buffer
	create_index_buffer();
}

// ===================== Buffer Creation =====================

void Fyredon::Mesh::create_vertex_buffers(const std::vector<float> &serial_verts)
{
	// Delete previous resources 
	if (VAO) glDeleteVertexArrays(1, &VAO);
	if (VBO) glDeleteBuffers(1, &VBO);

	// Gen VAO and VBO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Fill with mesh data (Assumes Mesh is in correct layout within mesh_data float array)
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, (vert_count * 12 * sizeof(float)), serial_verts.data(), GL_STATIC_DRAW);

	// Vertex Attribute 
	// Position (0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);
	// Normals (1)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), reinterpret_cast<void*>(sizeof(float) * 3));
	glEnableVertexAttribArray(1);
	// Colours (2)
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), reinterpret_cast<void*>(sizeof(float) * 6));
	glEnableVertexAttribArray(2);
	// UVs (3)
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), reinterpret_cast<void*>(sizeof(float) * 9));
	glEnableVertexAttribArray(3);

	// Clear bound state
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	flags.buffers_set = 1;
}

void Fyredon::Mesh::create_index_buffer()
{
	if (!indexed) {
		std::cerr << "WARNING::Trying to create index buffer on triangle soup primitive" << std::endl;
		return; 
	}

	if (index_count == 0 || inds_data.size() == 0) {
		std::cerr << "WARNING::No Indices Provided" << std::endl;
		return;
	}

	// Delete previous resources 
	if (EBO) glDeleteBuffers(1, &EBO);

	// Gen EBO
	glGenBuffers(1, &EBO);

	// Fill Index Buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, inds_data.size() * sizeof(GLuint), inds_data.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// ===================== Mesh Data Update =====================
// Assumes Data has been set and buffers created.

// Info : Only sets position of vertices (allows for updating positions per tick)
void Fyredon::Mesh::update_data_position(const std::vector<glm::vec3>& pos_data)
{
	assert(vert_count == pos_data.size() == vert_data.size());
	assert(flags.vert_data_set && flags.buffers_set);

	for (std::size_t v = 0; v < vert_count; ++v)
	{
		vert_data[v].pos = pos_data[v];
	}

	// Kinda slow, but saves storing multiple data arrays in memory
	set_data_mesh(vert_data);
}

// Info : Only sets colour of vertcies (allows for updating colours per tick)
void Fyredon::Mesh::update_data_colour(const std::vector<glm::vec3> &col_data)
{
	assert(vert_count == col_data.size());
	assert(flags.vert_data_set && flags.buffers_set);

	for (std::size_t v = 0; v < vert_count; ++v)
	{
		vert_data[v].col = col_data[v];
	}
	set_data_mesh(vert_data);
}

void Fyredon::Mesh::set_colour(const glm::vec3 &col)
{
	assert(flags.vert_data_set && flags.buffers_set);

	std::vector<glm::vec3> data(vert_count, col);
	update_data_colour(data);
}

// Info : Only support shader program using Vertex and Fragment pipeline currently
void Fyredon::Mesh::set_shader(const char *vert_path, const char *frag_path)
{
	// Create Shader
	std::string shader_name = name + "_Shader";
	shader = Shader(shader_name.c_str(), vert_path, frag_path);

	// Load and build shader
	shader.load();

	// Set Model Matrix
	shader.setMat4("model", model);
	if (shader.valid_state) flags.shader_set = 1; 
}

// Info : Forward Transforms to shader uniforms
void Fyredon::Mesh::set_cameraTransform(const glm::mat4x4 &view, const glm::mat4x4 &persp)
{
	if (!flags.shader_set) return; 
	shader.setMat4("view", view);
	shader.setMat4("proj", persp);
	flags.camera_set = 1;
}

// Flags that need to be set for rendering to be valid
bool Fyredon::Mesh::check_state() const
{
	if (indexed) return flags.buffers_set & flags.camera_set & flags.vert_data_set & flags.inds_data_set & flags.shader_set;
	return flags.buffers_set & flags.camera_set & flags.vert_data_set & flags.shader_set;
}

// ===================== Model Matrix Transformations =====================

void Fyredon::Mesh::translate(const glm::vec3 &offs)
{
	model = glm::translate(model, offs);
}

void Fyredon::Mesh::rotate(float d_ang, const glm::vec3 &axis)
{
	model = glm::rotate(model, glm::radians(d_ang), axis);
}

void Fyredon::Mesh::local_rotate(float x_ang, float y_ang, float z_ang)
{
	model = glm::rotate(model, glm::radians(x_ang), glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(y_ang), glm::vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(z_ang), glm::vec3(0, 0, 1));
}

void Fyredon::Mesh::scale(const glm::vec3 &scale)
{
	model = glm::scale(model, scale);
}

// ===================== Texturing =====================

void Fyredon::Mesh::load_texture(const char* filepath, GLuint unit)
{
	if (tex) delete tex; 
	std::string tex_name = name + "_texture";
	tex = new TextureImage(filepath, tex_name.c_str(), unit, true);
	tex->load();

	if (tex->valid_state) flags.texture_set = 1; else return;
	shader.setInt("tex", unit);
}

// ===================== Debug Code =====================
void Fyredon::Mesh::debug() const
{
	// Check for correct mesh_data size to vertex count with attributes. (11 * sizeof(float) per vert).
	assert((vert_data.size() * sizeof(float)) == (vert_count * sizeof(float) * 11));

	std::cout << "======== DEBUG::Camera::Mesh_" << name << "::Vertex_Data::BEGIN ========\n";
	std::size_t i = 0; 
	for (const Vert& vert : vert_data)
	{
		std::cout << "Vertex_" << i << "\n"
			<< "Pos =  [" << vert.pos.x    << ", " << vert.pos.y    << ","  << vert.pos.z    << "]\n"
			<< "Norm = [" << vert.normal.x << ", " << vert.normal.y << ","  << vert.normal.z << "]\n"
			<< "Col =  [" << vert.col.x    << ", " << vert.col.y    << ","  << vert.col.z    << "]\n"
			<< "Tex =  [" << vert.uv.x     << ", " << vert.uv.y     << ", " << vert.uv.z     << "\n";
		i++;
	}
	std::cout << "======== DEBUG::Camera::Mesh_" << name << "::Vertex_Data::END ========\n";
}