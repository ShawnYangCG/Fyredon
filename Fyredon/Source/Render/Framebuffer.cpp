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

// ====================== Framebuffer ABC Implementation ======================

Fyredon::Framebuffer_Base::Framebuffer_Base()
{
	glGenFramebuffers (1, &FBO_ID);
	glGenRenderbuffers(1, &RBO_ID);
}

Fyredon::Framebuffer_Base::~Framebuffer_Base()
{
	glDeleteFramebuffers (1, &FBO_ID);
	glDeleteRenderbuffers(1, &RBO_ID);
}

bool Fyredon::Framebuffer_Base::check() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO_ID);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cerr << "ERR::FrameBuffer " << FBO_ID << "Not Complete. Check Attachments.\n";
		return false; 
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return true; 
}


// ====================== Framebuffer_S Implementation ======================

Fyredon::Framebuffer_S::Framebuffer_S(Texture* ColourTexture)
	: Framebuffer_Base(), colour_texture(ColourTexture)
{
	// RBO Setup
	glBindRenderbuffer(GL_RENDERBUFFER, RBO_ID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, colour_texture->width, colour_texture->height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// Bind Colour Attachment Texture (ColAttach0) + RenderBuffer Depth Stencill
	glBindFramebuffer(GL_FRAMEBUFFER, FBO_ID);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colour_texture->ID, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO_ID);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Fyredon::Framebuffer_S::bind() 
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO_ID);
}

void Fyredon::Framebuffer_S::unbind() 
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Fyredon::Framebuffer_S::bind_clear()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO_ID);
	glClearColor(0.15f, 0.15f, 0.15f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// ====================== Framebuffer_MS Implementation ======================

Fyredon::Framebuffer_MS::Framebuffer_MS(Texture_MultiSample* ColourTexture, std::size_t Samples)
	: Framebuffer_Base(), colour_texture(ColourTexture), sample_count(Samples)
{
	// RBO Setup
	glBindRenderbuffer(GL_RENDERBUFFER, RBO_ID);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, sample_count, GL_DEPTH24_STENCIL8, colour_texture->width, colour_texture->height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// Bind Colour Attachment Texture (ColAttach0) + RenderBuffer Depth Stencill
	glBindFramebuffer(GL_FRAMEBUFFER, FBO_ID);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, colour_texture->ID, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO_ID);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Fyredon::Framebuffer_MS::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO_ID);
}

void Fyredon::Framebuffer_MS::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Fyredon::Framebuffer_MS::bind_clear()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO_ID);
	glClearColor(0.15f, 0.15f, 0.15f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// Info : Blit Multisampled Framebuffer to Standard Framebuffer to resolve samples. 
void Fyredon::Framebuffer_MS::blit_to_framebuffer_s(Framebuffer_S* target_fbo)
{
	// First check target is in valid state
	if (!target_fbo->check()) {
		std::cerr << "ERROR::Cannot blit to target framebuffer in, invalid state\n";
		return; 
	}

	// Bind and Clear target_fbo
	target_fbo->bind_clear();
	target_fbo->unbind();

	// Peform blit (colour and depth only) 
	glBindFramebuffer(GL_READ_FRAMEBUFFER, FBO_ID);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, target_fbo->FBO_ID);
	glBlitFramebuffer(0, 0, colour_texture->width, colour_texture->height, 0, 0, target_fbo->colour_texture->width, target_fbo->colour_texture->height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
	glBlitFramebuffer(0, 0, colour_texture->width, colour_texture->height, 0, 0, target_fbo->colour_texture->width, target_fbo->colour_texture->height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

	// Clear all bound FBO state
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}