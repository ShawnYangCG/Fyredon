// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	Framebuffer.cpp: Basic Framebuffer with Colour Attachment only : Niall
=============================================================================*/
#include "Fyredon/Other/pch.h"
// Implements
#include "Fyredon/Render/Framebuffer.h"

#include "Fyredon/Render/Texture.h"

// GLEW
#include "GLEW/glew.h" 

Fyredon::Framebuffer::Framebuffer(Texture* ColourTexture)
	: colour_texture(ColourTexture)
{
	// Gen FBO
	glGenFramebuffers(0, &ID);
	// Bind Colour Attachment Texture (ColAttach0)
	glBindFramebuffer(GL_FRAMEBUFFER, ID);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colour_texture->ID, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Fyredon::Framebuffer::~Framebuffer()
{
	glDeleteFramebuffers(0, &ID);
}

void Fyredon::Framebuffer::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, ID);
}

void Fyredon::Framebuffer::clear()
{
	glBindFramebuffer(GL_FRAMEBUFFER, ID);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}