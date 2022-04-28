// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	Texture.h: Multiple Texture classes for use with OpenGL : Niall
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

/* ====================== Texture_Base Class ======================
   Info: ABC of Texture to implement for standard and multi-sampled textures. */

class Texture_Base
{
public:
	enum filter_type { NEAREST = 0, LINEAR };

public:
	Texture_Base(const char* Tex_Name, GLuint Tex_Unit, std::size_t Width, std::size_t Height);
	virtual ~Texture_Base();

	virtual void set_params(filter_type filter) = 0;
	virtual void bind()   = 0;
	virtual void unbind() = 0;
	void activate();
	void deactivate();

	ImTextureID get_imgui_texture();

public:
	std::string name;
	filter_type filter;
	GLuint ID, unit;
	int32_t width, height, channels;
	bool valid_state;

};

/* ====================== Texture Class ======================
   Info: Standard Texture class, typically used for Colour or MS Sample Resolve attachments. */

class Texture : public Texture_Base
{
public:
	Texture(const char* Tex_Name, GLuint Tex_Unit, std::size_t Width, std::size_t Height);
	Texture();
	virtual ~Texture() = default;

	virtual void set_params(filter_type filter) override; 
	virtual void bind()   override;
	virtual void unbind() override;
};

// ========== Multisample Texture (only for use with FBO) ==========

/* ====================== Multisample Texture Class ======================
   Info: MultiSampled Texture class, used for Multisampled Framebuffer colour attachments. */

class Texture_MultiSample : public Texture_Base
{
public:
	Texture_MultiSample(const char* Tex_Name, GLuint Tex_Unit, std::size_t Width, std::size_t Height, std::size_t Samples);
	Texture_MultiSample() = delete; 
	virtual ~Texture_MultiSample() = default;

	virtual void set_params(filter_type filter) override;
	virtual void bind()   override;
	virtual void unbind() override;

public:
	std::size_t sample_count; 
};


/* ====================== Texture Image class (from file) ======================
   Info: Standard Texture that is defined by an image loaded from file (channel count of 3|4 only) */


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