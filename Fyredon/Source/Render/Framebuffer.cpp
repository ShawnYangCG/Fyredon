// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	Framebuffer.cpp: Basic Framebuffer with Colour Attachment only : Niall
=============================================================================*/
#include "Fyredon/Other/pch.h"
// Implements
#include "Fyredon/Render/Framebuffer.h"

// Project 
#include "Fyredon/Render/Texture.h"

// GLEW
#include "GLEW/glew.h" 

Fyredon::Framebuffer::Framebuffer(Texture* ColourTexture)
	: colour_texture(ColourTexture)
{
	// Gen FBO
	glGenFramebuffers(1, &FBO_ID);
	// Gen RBO
	glGenRenderbuffers(1, &RBO_ID);

	// RBO Setup
	glBindRenderbuffer(GL_RENDERBUFFER, RBO_ID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, colour_texture->width, colour_texture->height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// Bind Colour Attachment Texture (ColAttach0) + RenderBuffer
	glBindFramebuffer(GL_FRAMEBUFFER, FBO_ID);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colour_texture->ID, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO_ID);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Fyredon::Framebuffer::~Framebuffer()
{
	glDeleteFramebuffers(0,  &FBO_ID);
	glDeleteRenderbuffers(0, &RBO_ID);
}

void Fyredon::Framebuffer::bind() const 
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO_ID);
}

void Fyredon::Framebuffer::unbind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Fyredon::Framebuffer::bind_clear() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO_ID);
	glClearColor(0.15f, 0.15f, 0.15f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}