// Copyright Fyredon. All Rights Reserved.
/*=============================================================================
	ground.frag : Fragment Shader for Ground Plane Grid : Niall
=============================================================================*/

#version 400 core 

#define debug 0

in vec3 colour; 
in vec3 poscol;
in vec3 position_view; 
in vec2 uv; 

out vec4 frag_colour; 

uniform sampler2D tex; 

void main()
{
	#if debug == 0
	// Use filtered coverage without a hard cutoff, preserving subpixel grid lines.
	float coverage = texture(tex, uv).r;
	float fade = 1.0 - smoothstep(10.0, 30.0, length(position_view));
	frag_colour = vec4(vec3(1.0), coverage * fade);
	#else
	// Render UV Coords 
	frag_colour = vec4(uv.x, uv.y, 0.0, 1.0);
	#endif
}
