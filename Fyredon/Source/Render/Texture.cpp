// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	Texture.cpp: Basic texture loading and state management class : Niall
=============================================================================*/
#include "Fyredon/Other/pch.h"
// Implements 
#include "Fyredon/Render/Texture.h"

// Std Headers
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <vector>
#include <limits>

// Ext Headers
// GLEW
#include "GLEW/glew.h" 
// stb (defined here, nowhere else)
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#pragma warning( disable : 26812) // Ignore unscoped Enums 

// ====================== Texture_Base Implementation ======================

Fyredon::Texture_Base::Texture_Base(const char* Tex_Name, GLuint Tex_Unit, std::size_t Width, std::size_t Height)
	: name(Tex_Name), unit(Tex_Unit), width(Width), height(Height), channels(3)
{
	valid_state = false;
	ID = -1;
	filter = filter_type::NEAREST;
}

Fyredon::Texture_Base::~Texture_Base()
{
	glDeleteTextures(1, &ID);
}

void Fyredon::Texture_Base::activate()
{
	if (unit > 32) std::cerr << "ERROR::Fyredon::Texture::" << name << ":: texture unit out of bounds" << std::endl;
	glActiveTexture(0x84C0 + unit);
}

void Fyredon::Texture_Base::deactivate() 
{
	glActiveTexture(0); 
}

ImTextureID Fyredon::Texture_Base::get_imgui_texture()
{
	return reinterpret_cast<ImTextureID>(ID);
}

// ====================== Texture Class Implementation ======================

Fyredon::Texture::Texture(const char* Tex_Name, GLuint Tex_Unit, std::size_t Width, std::size_t Height)
	: Texture_Base(Tex_Name, Tex_Unit, Width, Height)
{
	// Gen Tex
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Set Parameters
	set_params(filter);
}

// Default Constructor for fallback only.
Fyredon::Texture::Texture()
	: Texture_Base("Null", -1, 0, 0)
{
	// Gen Tex
	glGenTextures(1, &ID);
	std::cout << "Warning, Default Texture Construction\n";
}

void Fyredon::Texture::set_params(filter_type filter)
{
	glBindTexture(GL_TEXTURE_2D, ID);
	switch (filter)
	{
	case 0:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		break;

	case 1:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		break;

	default:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	// Enforce mirrored repat. 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // X (S)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Y (T)
	glBindTexture(GL_TEXTURE_2D, 0);

	valid_state = true; 
}

void Fyredon::Texture::bind() { glBindTexture(GL_TEXTURE_2D, ID); }

void Fyredon::Texture::unbind() { glBindTexture(GL_TEXTURE_2D, 0); }

// ====================== Texture_MultiSample Implementation ======================

Fyredon::Texture_MultiSample::Texture_MultiSample(const char* Tex_Name, GLuint Tex_Unit, std::size_t Width, std::size_t Height, std::size_t Samples)
	: Fyredon::Texture_Base(Tex_Name, Tex_Unit, Width, Height), sample_count(Samples)
{
	// Gen Tex
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, ID);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, sample_count, GL_RGB, width, height, GL_TRUE);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

	// Set Parameters
	set_params(filter);
	valid_state = true;
}

// Info : Multisampled textures have no supported params. 
void Fyredon::Texture_MultiSample::set_params(filter_type filter) { return; }

void Fyredon::Texture_MultiSample::bind() { glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, ID); }

void Fyredon::Texture_MultiSample::unbind() { glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0); }

// ====================== TextureImage Class Implementation ======================

Fyredon::TextureImage::TextureImage(const char* File_Path, const char* Tex_Name, GLuint Tex_Unit, bool Flip)
	: Fyredon::Texture(Tex_Name, Tex_Unit, 0, 0), file_path(File_Path), flip(Flip) {}

// Info : Do loading after construction
void Fyredon::TextureImage::load()
{
	// Paths are UTF-8 at the API boundary. filesystem opens a Unicode path on Windows.
	std::ifstream file(std::filesystem::u8path(file_path), std::ios::binary | std::ios::ate);
	if (!file) throw std::runtime_error("Texture file could not be opened.");
	const auto size = file.tellg();
	if (size <= 0 || size > (std::numeric_limits<int>::max)())
		throw std::runtime_error("Texture file is empty or too large.");
	std::vector<unsigned char> encoded(static_cast<std::size_t>(size));
	file.seekg(0);
	if (!file.read(reinterpret_cast<char*>(encoded.data()), static_cast<std::streamsize>(encoded.size())))
		throw std::runtime_error("Texture file could not be read.");
	stbi_set_flip_vertically_on_load(flip);
	int originalChannels = 0;
	byte* tex_data = stbi_load_from_memory(encoded.data(), static_cast<int>(encoded.size()),
		&width, &height, &originalChannels, 4);
	channels = 4;
	// Check for invalid state
	if (!tex_data)
	{
		std::cerr << "ERROR::Fyredon::Texture::" << name << ":: Failed to load texture." << std::endl;
		throw std::runtime_error("Texture Load Failure\n");
	}
	if (channels < 3)
	{
		std::cerr << "ERROR::Fyredon::Texture::" << name << ":: Incorrect number of channels." << std::endl;
		throw std::runtime_error("Texture Load Failure\n");
	}

	// Gen Texture and Bind
	glBindTexture(GL_TEXTURE_2D, ID);
	switch (channels)
	{
	case 3:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, static_cast<void*>(tex_data));
		break;
	case 4:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, static_cast<void*>(tex_data));
	}
	stbi_image_free(tex_data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	set_params(filter);
	valid_state = true;
}
