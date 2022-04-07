// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	Texture.h: Basic texture loading and state management class : Niall
=============================================================================*/
#pragma once

// Std Headers
#include <iostream>
#include <string>

namespace Fyredon {

using GLuint = unsigned int;

// ========== Base Texture class, (used for Framebuffer Texture State) ==========

class Texture
{
public:
	enum filter_type { NEAREST = 0, LINEAR };
public:
	Texture(const char* Tex_Name, GLuint Tex_Unit);
	Texture() = default;
	virtual ~Texture();

	virtual void set_params(filter_type filter);
	virtual void activate();
	virtual void bind();

public:
	std::string name;
	filter_type filter;
	GLuint ID, unit;
	int32_t width, height, channels;
	bool valid_state;

};

// ========== Texture Image class (from file) ==========

class TextureImage : Texture
{
public:
	enum filter_type { NEAREST = 0, LINEAR };
public:
	TextureImage(const char* File_Path, const char* Tex_Name, GLuint Tex_Unit);
	TextureImage() = default;
	virtual ~TextureImage() = default;

	void load();

public:
	bool flip;
	std::string file_path;
};





}