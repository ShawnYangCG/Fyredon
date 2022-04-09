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

// Ext Headers
// GLEW
#include "GLEW/glew.h" 
// stb (defined here, nowhere else)
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#pragma warning( disable : 26812) // Ignore unscoped Enums 

// ====================== Texture Class Implementation ======================

Fyredon::Texture::Texture(const char* Tex_Name, GLuint Tex_Unit, std::size_t Width, std::size_t Height)
	: name(Tex_Name), unit(Tex_Unit), width(Width), height(Height), channels(3)
{
	valid_state = false;
	ID = -1;
	filter = filter_type::NEAREST;

	// Gen Tex
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	set_params(filter);
}

// Default Constructor for fallback only.
Fyredon::Texture::Texture() 
	: name("Null"), unit(-1), width(1), height(1), channels(3)
{
	// Gen Tex
	glGenTextures(1, &ID);
	std::cout << "Warning, Default Texture Construction\n";
}

Fyredon::Texture::~Texture()
{
	glDeleteTextures(1, &ID);
}

void Fyredon::Texture::set_params(filter_type filter)
{
	glBindTexture(GL_TEXTURE_2D, ID);
	switch (filter)
	{
		case 0 : 
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			break;

		case 1 : 
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
}

void Fyredon::Texture::activate()
{
	switch (unit)
	{
		case 0:	
			glActiveTexture(GL_TEXTURE0);
			break;
		case 1:	
			glActiveTexture(GL_TEXTURE1);
			break;
		case 2:	
			glActiveTexture(GL_TEXTURE2);
			break;
		case 3:
			glActiveTexture(GL_TEXTURE3);
			break;
		default: std::cerr << "ERROR::Fyredon::Texture::" << name << ":: texture unit out of bounds" << std::endl;
	}
}

void Fyredon::Texture::bind()
{
	glBindTexture(GL_TEXTURE_2D, ID);
}

// ====================== TextureImage Class Implementation ======================

Fyredon::TextureImage::TextureImage(const char* File_Path, const char* Tex_Name, GLuint Tex_Unit)
	: Fyredon::Texture(Tex_Name, Tex_Unit, -1, -1), file_path(File_Path), flip(true) 
{}

// Info : Do loading after construction
void Fyredon::TextureImage::load()
{
	// Check File path
	std::ifstream file(file_path);
	if (!file.is_open())
	{
		std::cerr << "ERROR::Fyredon::Texture::" << name << ":: file does not exist." << std::endl;
		std::terminate();
	}
	file.close();

	// Load via Stb image : 
	stbi_set_flip_vertically_on_load(flip); // Flip Texture Y Axis on Load. 
	byte* tex_data = stbi_load(file_path.c_str(), &width, &height, &channels, 0);

	// Check for invalid state
	if (!tex_data)
	{
		std::cerr << "ERROR::Fyredon::Texture::" << name << ":: Failed to load texture." << std::endl;
		return;
	}
	if (channels < 3)
	{
		std::cerr << "ERROR::Fyredon::Texture::" << name << ":: Incorrect number of channels." << std::endl;
		return;
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
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	set_params(filter);
	valid_state = true;
}
