// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	Texture.h: Basic texture loading and state management class : Niall
=============================================================================*/
#pragma once

// Std Headers
#include <iostream>
#include <string>

#ifndef ImTextureID
typedef void* ImTextureID;       
#endif

namespace Fyredon {

using GLuint = unsigned int;

// ========== Base Texture class, (used for Framebuffer Texture State) ==========

class Texture
{
public:
	enum filter_type { NEAREST = 0, LINEAR };

public:
	Texture(const char* Tex_Name, GLuint Tex_Unit, std::size_t Width, std::size_t Height);
	Texture();
	virtual ~Texture();

	virtual void set_params(filter_type filter);
	void activate();
	void deactivate();
	void bind();
	void unbind();
	ImTextureID get_imgui_texture();


public:
	std::string name;
	filter_type filter;
	GLuint ID, unit;
	int32_t width, height, channels;
	bool valid_state;

};

// ========== Multisample Texture (only for use with FBO) ==========

class Texture_MultiSample : public Texture
{
public:
	Texture_MultiSample(const char* Tex_Name, GLuint Tex_Unit, std::size_t Width, std::size_t Height, std::size_t Samples);
	Texture_MultiSample() = delete; 
	virtual ~Texture_MultiSample() = default;

	virtual void set_params(filter_type filter) override;

	std::size_t sample_count; 
};


// ========== Texture Image class (from file) ==========

class TextureImage : public Texture
{
public:
	enum filter_type { NEAREST = 0, LINEAR };
public:
	TextureImage(const char* File_Path, const char* Tex_Name, GLuint Tex_Unit, bool Flip);
	TextureImage() = delete; 
	virtual ~TextureImage() = default;

	void load();

public:
	bool flip;
	std::string file_path;
};


}