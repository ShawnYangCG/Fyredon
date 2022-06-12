// Copyright Fyredon. All Rights Reserved.
/*=============================================================================
	terrain.frag : 
=============================================================================*/

#version 400 core 

// Input
in vec3 colour;
in vec3 normal;
in vec3 uvw;
in vec3 pos_world;
in vec3 normal_world; 

// Uniforms 
uniform vec3 camPos_world; 
uniform vec3 lightPos_world;
uniform float lightStr; 
uniform bool ren_normals; 

uniform sampler2D tex; 

// Output 
out vec4 frag_colour; 

void main()
{
	float height = colour.x; 
	vec3 tcol = mix(vec3(0.02, 0.1, 0.75), vec3(0.1, 0.75, 0.2), min(height * 10.0, 1.0)); 
	
	float water_mask = 1.0 - (min(height * 10.0, 1.0));
	
	// Calc Blinn-Phong (WS) (Single Light Source)
	vec3 light_dir    = normalize(lightPos_world - pos_world); 
	vec3 view_dir     = normalize(camPos_world - pos_world);
	vec3 half_v       = normalize(light_dir + view_dir);
	//vec3 r_colour     = vec3(0.2, 0.2, 0.2);
	vec3 r_colour = vec3(texture(tex, vec2(uvw.x * 1.f, uvw.y * 1.f)));  
	r_colour = mix(vec3(0.02, 0.1, 0.75), r_colour, min(height * 10.0, 1.0)); 
	//r_colour = 
	vec3 a = 0.125 * r_colour; 
	vec3 d = (max(dot(normal_world, light_dir), 0.0) * r_colour) * lightStr; 
	vec3 s = (pow(max(dot(normal_world, half_v), 0.0), 12.0) * vec3(0.98,0.98,0.98)) * lightStr; 
	
	vec3 o = a + d + (water_mask * s); 
	frag_colour = vec4(o, 1.0); 
}
