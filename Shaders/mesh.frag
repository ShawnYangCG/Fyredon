// Copyright Fyredon. All Rights Reserved.
/*=============================================================================
	mesh.frag : Mesh Fragment Shader : Niall
=============================================================================*/

#version 400 core 

// Input
in vec3 colour;
in vec3 normal;
in vec3 pos_world;
in vec3 normal_world; 

// Uniforms 
uniform vec3 camPos_world; 
uniform vec3 lightPos_world;
uniform float lightStr; 
uniform bool ren_normals; 

// Output 
out vec4 frag_colour; 

void main()
{
	// Calc Blinn-Phong (WS) (Single Light Source)
	vec3 light_dir    = normalize(lightPos_world - pos_world); 
	vec3 view_dir     = normalize(camPos_world - pos_world);
	vec3 half_v       = normalize(light_dir + view_dir);
	vec3 r_colour     = colour;
	vec3 a = 0.125 * r_colour; 
	vec3 d = (max(dot(normal_world, light_dir), 0.0) * r_colour) * lightStr; 
	vec3 s = (pow(max(dot(normal_world, half_v), 0.0), 4.0) * vec3(0.95,0.95,0.95)) * lightStr; 
	
	frag_colour = vec4(a+d+s, 1.0); 	
}
