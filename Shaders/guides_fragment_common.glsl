// Copyright Fyredon. All Rights Reserved.
/*=============================================================================
	guides.frag : Guide Meshes Fragment Shader : Niall
=============================================================================*/
//#version 450 core 

// ======= Input ======= 
in vec3 colour;
in vec3 uvw;
in vec3 pos_world;
in vec3 normal_world; 

// ======= Output ======= 
out vec4 frag_colour; 

void main()
{
	frag_colour = vec4(colour, 1.0);
}
