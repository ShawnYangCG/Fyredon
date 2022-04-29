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
	vert_path = vertpath, frag_path = fragpath; 
} 

// ToDo : Look into this seg fault ...
Fyredon::Shader::~Shader()
{
	//glDeleteProgram(ID);
}

void Fyredon::Shader::load()
{
	std::ifstream vShaderFile, fShaderFile;
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		vShaderFile.open(vert_path);
		fShaderFile.open(frag_path);

		std::stringstream vShaderStream, fShaderStream;

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vert_shader_code = vShaderStream.str();
		frag_shader_code = fShaderStream.str();
	}
	catch (std::ifstream::failure err) // Catch ifstream failure error to object err
	{
		std::cerr << "ERROR::Fyredon::Shader::" << name << "::Failed to load shader code::" << err.what() << std::endl;
		std::terminate();
	}

	const char *vShaderCode = vert_shader_code.c_str();
	const char *fShaderCode = frag_shader_code.c_str();

	// Intermediate Shaders (Vertex, Fragment) handles. 
	unsigned int vertexS, fragmentS;

	// Build Vertex Shader
	vertexS = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexS, 1, &vShaderCode, NULL);
	glCompileShader(vertexS);
	check_compile(vertexS, "Vertex_Shader");

	// Build Fragment Shader
	fragmentS = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentS, 1, &fShaderCode, NULL);
	glCompileShader(fragmentS);
	check_compile(fragmentS, "Fragment_Shader");

	// Build Shader Program
	ID = glCreateProgram();
	glAttachShader(ID, vertexS);
	glAttachShader(ID, fragmentS);
	glLinkProgram(ID);
	check_link();

	// Delete Intermediate Shaders
	glDeleteShader(vertexS);
	glDeleteShader(fragmentS);

	valid_state = true;
}

void Fyredon::Shader::use()
{
	if (!valid_state) {std::cerr << "ERROR::Fyredon::Shader::" << name << ":: invalid state" << std::endl; return;}
	glUseProgram(ID);
}

// ========================== Set Uniform Member Functions ========================== 
void Fyredon::Shader::setBool(const std::string &name, bool value) const
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

void Fyredon::Shader::setInt(const std::string &name, int value) const
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

void Fyredon::Shader::setFloat(const std::string &name, float value) const
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

void Fyredon::Shader::setVec(const std::string &name, const glm::vec3 value) const
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

void Fyredon::Shader::setMat3(const std::string &name, const glm::mat3x3 value) const
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

void Fyredon::Shader::setMat4(const std::string &name, const glm::mat4x4 value) const
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