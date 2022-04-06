// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	Texture.h: Basic texture loading and state management class : Niall
=============================================================================*/
#pragma once

// Std Headers
#include <iostream>
#include <string>
#include <sstream>

namespace Fyredon {

using GLuint = unsigned int;

class Texture
{
public:
	enum filter_type { NEAREST = 0, LINEAR };
public:
	Texture(const char* Tex_Name, const char* Tex_Path, GLuint Tex_Unit);
	Texture() = default;
	~Texture();

	// State Setup
	void load();
	void set_params(filter_type filter);

	// Tick Calls
	void activate();
	void bind();

	std::ostringstream debug();

public:
	bool valid_state;
	int32_t width, height, channels;
	std::string name, file_path;
	GLuint ID, unit;
	filter_type filter;
};

}