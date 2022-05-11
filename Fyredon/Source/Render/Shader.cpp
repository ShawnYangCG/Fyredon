// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	Shader.cpp: Load, Compile, Link shaders to define shadw program : Niall
=============================================================================*/
#include "Fyredon/Other/pch.h"
// Implements 
#include "Fyredon/Render/Shader.h" 

// Ext Headers
// GLEW
#include "GLEW/glew.h" 
// GLM
#include "glm/gtc/type_ptr.hpp"

// Std Headers
#include <iostream>
#include <fstream>
#include <sstream>

#define BUFFER_SIZE 512

// Creation of Shader, Loading and Building, now decoupled from Ctor, for completeness of OpenGL state. 

// ToDo : Replace Logger output with Debug Class

Fyredon::Shader::Shader(const char *sname, const char *vertpath, const char *fragpath)
{
	name = sname; 
	valid_state = false;
	flags.loaded = false, flags.built = false; 
	vert_path = vertpath, frag_path = fragpath; 
} 

// ToDo : Look into this seg fault ...
Fyredon::Shader::~Shader()
{
	//glDeleteProgram(ID);
}

void Fyredon::Shader::load()
{
	flags.loaded = false, valid_state = false;

	// Clear any existing state
	if (vert_shader) glDeleteShader(vert_shader); 
	if (frag_shader) glDeleteShader(frag_shader); 
	if (vert_shader_code.size()) vert_shader_code.clear(); 
	if (frag_shader_code.size()) frag_shader_code.clear(); 

	std::ifstream vShaderFile, fShaderFile;
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		std::stringstream vShaderStream, fShaderStream;

		vShaderFile.open(vert_path); fShaderFile.open(frag_path);

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close(); fShaderFile.close();

		vert_shader_code = vShaderStream.str();
		frag_shader_code = fShaderStream.str();
	}
	catch (std::ifstream::failure err) // Catch ifstream failure error to object err
	{
		std::cerr << "ERROR::Fyredon::Shader::" << name << "::Failed to load shader code::" << err.what() << std::endl;
		throw std::runtime_error("Shader Loader Error");
	}

	vert_shader = glCreateShader(GL_VERTEX_SHADER);
	frag_shader = glCreateShader(GL_FRAGMENT_SHADER);

	flags.loaded = true; 
}

void Fyredon::Shader::build(std::vector<const char*>& frag_defines)
{
	flags.built = false, valid_state = false;
	//if (ID) glDeleteProgram(ID);

	assert(flags.loaded); // Source needs to be loaded first. 

	const char* vShaderCode = vert_shader_code.c_str();
	const char* fShaderCode = frag_shader_code.c_str();

	// Compile Vertex Shader
	glShaderSource(vert_shader, 1, &vShaderCode, NULL);
	glCompileShader(vert_shader);
	check_compile(vert_shader, "Vertex_Shader");

	// Compile Fragment Shader
	if (!frag_defines.size())
	{
		glShaderSource(frag_shader, 1, &fShaderCode, NULL);
		glCompileShader(frag_shader);
		check_compile(frag_shader, "Fragment_Shader");
	}
	else // Pass preprocessor define strings to prepend shader source
	{
		// [0]         : Version Directive
		// [1 - (n-1)] : Branching Directives (from material)
		// [n]         : Shader File Code
		frag_defines.insert(frag_defines.begin(), "#version 450 core\n");
		frag_defines.push_back(fShaderCode);
		glShaderSource(frag_shader, frag_defines.size(), frag_defines.data(), NULL);
		glCompileShader(frag_shader);
		check_compile(frag_shader, "Fragment_Shader");
	}

	// Build Shader Program
	ID = glCreateProgram();
	glAttachShader(ID, vert_shader);
	glAttachShader(ID, frag_shader);
	glLinkProgram(ID);
	check_link();

	flags.built = true; valid_state = true;
}

void Fyredon::Shader::use()
{
	if (!valid_state) {std::cerr << "ERROR::Fyredon::Shader::" << name << ":: invalid state" << std::endl; return;}
	glUseProgram(ID);
}

// ========================== Set Uniform Member Functions ========================== 
// NOTE : Invalid Uniforms may have been optimised out of the shader due to not been used, hence an error limit is imposed
//        to avoid repetitve error messages. However, always ensure the uniform has been set if its inteded for use. 

void Fyredon::Shader::set_bool(const std::string &name, bool value) const
{
	static std::size_t count = 0; 
	glUseProgram(ID);
	GLint id = glGetUniformLocation(ID, name.c_str());
	if (!id){ 
		if (count > 1) return;
		count++;
		std::cerr << "ERROR::Fyredon::Shader::" << this->name << ":: invalid uniform " << name << "\n";
		return;}
	glUniform1i(id, (int)value); 
}

void Fyredon::Shader::set_int(const std::string &name, int value) const
{
	static std::size_t count = 0;
	glUseProgram(ID);
	GLint id = glGetUniformLocation(ID, name.c_str());
	if (id == -1) { 
		if (count > 1) return;
		count++;
		std::cerr << "ERROR::Fyredon::Shader::" << this->name << ":: invalid uniform " << name << "\n";
		return; }
	glUniform1i(id, value);
}

void Fyredon::Shader::set_float(const std::string &name, float value) const
{
	static std::size_t count = 0;
	glUseProgram(ID);
	GLint id = glGetUniformLocation(ID, name.c_str());
	if (id == -1) { 
		if (count > 1) return;
		count++;
		std::cerr << "ERROR::Fyredon::Shader::" << this->name << ":: invalid uniform " << name << "\n"; 
		return; }
	glUniform1f(id, value);
}

void Fyredon::Shader::set_vec3(const std::string &name, const glm::vec3 &value) const
{
	static std::size_t count = 0;
	glUseProgram(ID);
	GLint id = glGetUniformLocation(ID, name.c_str());
	if (id == -1) { 
		if (count > 1) return;
		count++;
		std::cerr << "ERROR::Fyredon::Shader::" << this->name << ":: invalid uniform " << name << "\n";
		return; }
	glUniform3fv(id, 1, glm::value_ptr(value));
}

void Fyredon::Shader::set_vec4(const std::string& name, const glm::vec4 &value) const
{
	static std::size_t count = 0;
	glUseProgram(ID);
	GLint id = glGetUniformLocation(ID, name.c_str());
	if (id == -1) {
		if (count > 1) return;
		count++;
		std::cerr << "ERROR::Fyredon::Shader::" << this->name << ":: invalid uniform " << name << "\n";
		return;
	}
	glUniform4fv(id, 1, glm::value_ptr(value));
}

void Fyredon::Shader::set_mat3(const std::string &name, const glm::mat3x3 &value) const
{
	static std::size_t count = 0; 
	glUseProgram(ID);
	GLint id = glGetUniformLocation(ID, name.c_str());
	if (id == -1) { 
		if (count > 1) return;
		count++;
		std::cerr << "ERROR::Fyredon::Shader::" << this->name << ":: invalid uniform " << name << "\n"; 
		return; }
	glUniformMatrix3fv(id, 1, GL_FALSE, glm::value_ptr(value));
}

void Fyredon::Shader::set_mat4(const std::string &name, const glm::mat4x4 &value) const
{
	static std::size_t count = 0; 
	glUseProgram(ID);
	GLint id = glGetUniformLocation(ID, name.c_str());
	if (id == -1) {
		if (count > 1) return;
		count++;
		std::cerr << "ERROR::Fyredon::Shader::" << this->name << ":: invalid uniform " << name << "\n"; 
		return; }
	glUniformMatrix4fv(id, 1, GL_FALSE, glm::value_ptr(value));
}


// ========================== Check Compile and Link ========================== 
void Fyredon::Shader::check_compile(unsigned int &shader, std::string type)
{
	int sucess;
	char err_log[BUFFER_SIZE];

	glGetShaderiv(shader, GL_COMPILE_STATUS, &sucess);

	if (!sucess)
	{
		glGetShaderInfoLog(shader, BUFFER_SIZE, NULL, err_log);
		std::cout << "ERROR::Fyredon::Shader::" << name << " " << type << "::Compile Failed : " 
			<< err_log << std::endl;
		std::terminate();
	}
}

void Fyredon::Shader::check_link()
{
	int sucess;
	char err_log[BUFFER_SIZE];

	glGetProgramiv(ID, GL_LINK_STATUS, &sucess);

	if (!sucess)
	{
		glGetProgramInfoLog(ID, BUFFER_SIZE, NULL, err_log);
		std::cerr << "ERROR::Fyredon::Shader::" << name << "::Linkage Failed : " 
			<< err_log << std::endl;
		std::terminate();
	}
}

// ========================== Debug / Output Shader Code ========================== 
void Fyredon::Shader::debug_vert()
{
	if (vert_shader_code.size() >= 1)
	{
		std::cout << "DEBUG::Shader_" << name 
			<< "\n==== Vertex Shader Code BEGIN ====\n" 
			<< vert_shader_code
			<< "\n==== Vertex Shader Code END ====\n";
	}
	else
	{
		std::cerr << "ERROR:Shader_" << name
			<< " Invalid Vertex Shader Code" << std::endl;
	}
}

void Fyredon::Shader::debug_frag()
{
	if (frag_shader_code.size() >= 1)
	{
		std::cout << "DEBUG::Shader_" << name
			<< "\n==== Fragment Shader Code BEGIN ====\n"
			<< frag_shader_code
			<< "\n==== Fragment Shader Code END ====\n";
	}
	else
	{
		std::cerr << "ERROR:Shader_" << name
			<< " Invalid Fragment Shader Code" << std::endl;
	}
}