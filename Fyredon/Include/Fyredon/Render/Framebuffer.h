// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	Framebuffer.h: .... : Niall
=============================================================================*/
#pragma once

namespace Fyredon {

class Texture; 
using GLuint = unsigned int;

// This is super simple, colour attachment only, no multi-sampiling. 
class Framebuffer
{
public:
	Framebuffer(Texture * ColourTexture);
	~Framebuffer(); 

	void bind()   const;
	void unbind() const;
	void bind_clear()  const;

	Texture* colour_texture; 
	GLuint FBO_ID, RBO_ID; 
};

}