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
// Camera
uniform vec3 cam_pos_world; 
// Sun directional light 
uniform vec3  s_colour; 
uniform vec3  s_direction; 
uniform float s_strength;  
// Material Texture Maps
uniform sampler2D m_tex_diffuse; 
uniform sampler2D m_tex_specular; 
// Material Parameters
uniform vec3  m_colour_ambient; 
uniform vec3  m_colour_diffuse;
uniform vec3  m_colour_specular; 
uniform float m_coeff_ambient;
uniform float m_coeff_diffuse; 
uniform float m_coeff_specular;
uniform float m_exp_gloss; 


void main()
{
	vec3 amb_col  = m_colour_ambient * m_coeff_ambient;
	vec3 diff_col = m_colour_diffuse; 
	vec3 spec_col = m_colour_specular;
	
	//frag_colour = vec4((s_colour + direction) * strength, 1.0); 
	//frag_colour = vec4(1.0, 0.0, 0.0, 1.0); 
	//frag_colour = vec4(normal_world, 1.0); 
	

	// Test Branch
	#ifdef BRANCH_LIGHT_SUN
	frag_colour = vec4(1.0, 0.0, 0.0, 1.0); 
	return; 
	#endif
	
	frag_colour = vec4(amb_col, 1.0); 
	
	/* --------------- DEBUGGING UNIFORMS...
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
	vec3 light_dir    = direction; 
	vec3 view_dir     = normalize(camPos_world - pos_world);
	vec3 half_v       = normalize(light_dir + view_dir);
	
	// Basic Lambertian diffuse term
	float lambertian = max(dot(normal_world, light_dir), 0.0);
	
	// Blinn Phong Specular Term
	float bpspec = pow(max(dot(normal_world, half_v), 0.0), mat_parms.exp_gloss);
		
	// Shading Terms
	vec3 ambient  = amb_col;
	vec3 diffuse  = lambertian * diff_col * s_colour * strength; 
	vec3 specular = bpspec * spec_col * strength;  

	// Linear output
	vec3 lin_col = ambient + diffuse + specular; 
	
	// Gamma correction
	vec3 out_c = pow(lin_col, vec3(1.0 / 2.2)); 
	
	// Output
	frag_colour = vec4(out_c, 1.0); 
	*/
}
