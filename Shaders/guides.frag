// Copyright Fyredon. All Rights Reserved.
/*=============================================================================
	guides.frag : Guides Geo Fragment Shader : Niall
=============================================================================*/

#version 400 core 

in vec3 colour;
out vec4 frag_colour; 

void main()
{
	frag_colour = vec4(colour, 1.0);
}
