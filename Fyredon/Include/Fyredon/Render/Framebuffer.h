// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	Framebuffer.h: Standard and Multisampled Framebuffers for OpenGL : Niall
=============================================================================*/
#pragma once

namespace Fyredon {

class Texture; 
class Texture_MultiSample;
using GLuint = unsigned int;

/* ====================== Frambeuffer_Base ======================
   Info: ABC of framebuffer to implement for standard and multi-sampled framebuffers. */

class Framebuffer_Base
{
public:
	Framebuffer_Base();
	virtual ~Framebuffer_Base();

	virtual void bind() = 0;
	virtual void unbind() = 0;
	virtual void bind_clear() = 0;

	bool check() const; 

public:
	GLuint FBO_ID, RBO_ID;
};

/* ====================== Frambeuffer_Standard ======================
   Info: Framebuffer that writes to a :
   * Standard Texture colour attachment and
   * Standard Render Buffer depth+stencil attachment. 
   Note: Colour Attachment Texture is passed to framebuffer, not created within (unlike RBO). */

class Framebuffer_S final : public Framebuffer_Base
{
public:
	Framebuffer_S(Texture* ColourTexture);
	virtual ~Framebuffer_S() = default;

	virtual void bind()        override;
	virtual void unbind()      override;
	virtual void bind_clear()  override;

public:
	Texture* colour_texture;
};

/* ====================== Frambeuffer_Multisampled ======================
   Info: Framebuffer that writes to a : 
   * Multisampled Texture colour attachment and 
   * Multisampled Tender Buffer depth+stencil attachment.
   Note: Colour Attachment Texture is passed to framebuffer, not created within (unlike RBO). */

class Framebuffer_MS final : public Framebuffer_Base
{
public:
	Framebuffer_MS(Texture_MultiSample* ColourTexture, std::size_t Samples);
	virtual ~Framebuffer_MS() = default;

	virtual void bind()        override;
	virtual void unbind()      override;
	virtual void bind_clear()  override;

	void blit_to_framebuffer_s(Framebuffer_S* target_fbo);

public:
	Texture_MultiSample* colour_texture;
	std::size_t sample_count; 
};

}