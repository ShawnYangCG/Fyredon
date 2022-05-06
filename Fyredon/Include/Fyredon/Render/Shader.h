// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	Shader.h: Load, Compile, Link shaders to define shadw program : Niall
=============================================================================*/
#pragma once

// Ext Headers
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

// Std Headers
#include <string>
#include <vector>

namespace Fyredon
{
	using GLuint = unsigned int; 

// Info : Basic Shader class, that defines a shader program containg a Vertex and Fragment stage.
//        This will be replaced with base shader classes later. 

class Shader
{
public:
	Shader() = default;
	Shader(const char *name, const char *vertparth, const char *fragpath);
	~Shader();

	void load();
	void build(std::vector<const char*> &frag_defines);
	void use();

	// Set Shader Uniforms
	void set_bool (const std::string &name, bool value)                const;
	void set_int  (const std::string &name, int value)                 const;
	void set_float(const std::string &name, float value)               const;
	void set_vec3 (const std::string &name, const glm::vec3 &value)    const;
	void set_vec4 (const std::string &name, const glm::vec4 &value)    const;
	void set_mat3 (const std::string &name, const glm::mat3x3 &value)  const;
	void set_mat4 (const std::string &name, const glm::mat4x4 &value)  const;

	// Debug
	void check_compile(unsigned int &shader, std::string type);
	void check_link();
	void debug_vert();
	void debug_frag();

public:
	std::string name;
	GLuint ID; 
	GLuint vert_shader, frag_shader; 

	struct
	{
		int8_t loaded : 1;
		int8_t built : 1; 
	} flags;
	bool valid_state;

private:
	std::string vert_path, frag_path;
	std::string vert_shader_code, frag_shader_code;
};

}


