// Copyright Fyredon. All Rights Reserved.
/*=============================================================================
  blinn-phong_frag.glsl : Blinn-Phong BRDF based Uber Fragment Shader : Niall 
=============================================================================*/
//#version 450 core 

// Info :  For now use a single sun light only. 

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
uniform sampler2D tex_diffuse; 
uniform sampler2D tex_specular; 

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
	vec3 amb_col  = mat_parms.colour_ambient;
	vec3 diff_col = mat_parms.colour_diffuse; 
	vec3 spec_col = mat_parms.colour_specular;
	
	// Test Branch
	#ifdef BRANCH_LIGHT_SUN
	frag_colour = vec4(1.0, 0.0, 0.0, 1.0); 
	return; 
	#endif
	
	// Diffuse Colour 
	#ifdef BRANCH_DIFF_MAP
	diff_col = vec3(texture(textures.tex_diffuse, vec2(uvw.x, uvw.y))); 
	#endif
	#ifdef BRANCH_DIFF_VERTCOLOUR
	diff_col = colour;
	#endif
	
	// Specular Map
	#ifdef BRANCH_SPEC_MAP
	spec_col = spec_col * (texture(textures.tex_specular, vec2(uvw.x, uvw.y))).x; 
	#endif
	
	// Blinn_Phong BRDF (Sun only) (WS calculations)	
	// Sun Lighting Vectors
	vec3 light_dir    = sun.direction; 
	vec3 view_dir     = normalize(camPos_world - pos_world);
	vec3 half_v       = normalize(light_dir + view_dir);
	
	// Basic Lambertian diffuse term
	float lambertian = max(dot(normal_world, light_dir), 0.0);
	
	// Blinn Phong Specular Term
	float bpspec = pow(max(dot(normal_world, half_v), 0.0), mat_parms.exp_gloss);
		
	// Shading Terms
	vec3 ambient  = amb_col;
	vec3 diffuse  = lambertian * diff_col * sun.strength; 
	vec3 specular = bpspec * spec_col * sun.strength;  

	// Linear output
	vec3 lin_col = ambient + diffuse + specular; 
	
	// Gamma correction
	vec3 out_c = pow(lin_col, vec3(1.0 / 2.2)); 
	
	// Output
	frag_colour = vec4(out_c, 1.0); 
}
