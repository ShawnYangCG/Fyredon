// Copyright Fyredon. All Rights Reserved.
/*=============================================================================
	terrain.frag : Needs refactoring for material system. 
=============================================================================*/
#version 450 core 

// ======= Input ======= 
in vec3 colour;
in vec3 uvw;
in vec3 pos_world;
in vec3 normal_world; 

// ======= Output ======= 
out vec4 frag_colour; 

// ======= Uniforms ======= 
uniform vec3 camPos_world; 

// Sun directional light 
uniform light_directional
{
	vec3 colour; 
	vec3 direction; 
	float strength; 
} sun; 

// Texture Maps
uniform tex_block
{
	sampler2D tex_diffuse; 
	sampler2D tex_specular; 
} textures;

// Material Parameters
uniform material
{
	vec3 colour_ambient; 
	vec3 colour_diffuse;
	vec3 colour_specular; 
	
	float coeff_ambient;
	float coeff_diffuse; 
	float coeff_specular;
	float exp_gloss; 
} mat_parms; 


void main()
{
	float height = colour.x; 
	vec3 tcol = mix(vec3(0.02, 0.1, 0.75), vec3(0.1, 0.75, 0.2), min(height * 10.0, 1.0)); 
	float water_mask = 1.0 - (min(height * 10.0, 1.0));
	
	// Calc Blinn-Phong (WS) (Single Light Source)
	vec3 light_dir    = normalize(lightPos_world - pos_world); 
	vec3 view_dir     = normalize(camPos_world - pos_world);
	vec3 half_v       = normalize(light_dir + view_dir);

	vec3 r_colour = vec3(texture(textures.tex_diffuse, vec2(uvw.x * 5.f, uvw.y * 5.f)));  
	r_colour = mix(vec3(0.02, 0.1, 0.75), r_colour, min(height * 10.0, 1.0)); 
	
	vec3 a = 0.125 * r_colour; 
	vec3 d = (max(dot(normal_world, light_dir), 0.0) * r_colour) * lightStr; 
	vec3 s = (pow(max(dot(normal_world, half_v), 0.0), 12.0) * vec3(0.98,0.98,0.98)) * lightStr; 
	vec3 o = a + d + (water_mask * s); 
	
	o = pow(o, vec3(1.0 / 2.2)); 
	frag_colour = vec4(o, 1.0); 
}
