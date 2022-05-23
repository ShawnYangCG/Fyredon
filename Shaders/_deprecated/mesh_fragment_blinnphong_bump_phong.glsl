// Copyright Fyredon. All Rights Reserved.
/*=============================================================================
  blinn-phong_frag.glsl : Blinn-Phong BRDF based Uber Fragment Shader : Niall 
=============================================================================*/
//#version 450 core 

// Info :  For now use a single sun light only. 

// ======= Input ======= 
in vec3 colour;
in vec3 uvw;
in vec3 pos_proj; 
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
uniform sampler2D m_tex_bump; 
// Material Parameters
uniform vec3  m_colour_ambient; 
uniform vec3  m_colour_diffuse;
uniform vec3  m_colour_specular; 
uniform float m_coeff_ambient;
uniform float m_coeff_diffuse; 
uniform float m_coeff_specular;
uniform float m_exp_gloss; 
uniform float m_coeff_bump;


void main()
{
	vec3 amb_col  = m_colour_ambient;
	vec3 diff_col = m_colour_diffuse; 
	vec3 spec_col = m_colour_specular;
	float sun_strength = s_strength; 
	
	vec3 normal = normal_world; 
	
	// Directional Light (Sun)
	#ifndef BRANCH_LIGHT_SUN
	sun_strength = 0.0;
	#endif

	// Diffuse Colour 
	#ifdef BRANCH_DIFF_MAP
	diff_col = vec3(texture(m_tex_diffuse, vec2(uvw.x, uvw.y))); 
	#endif
	#ifdef BRANCH_DIFF_VERTCOLOUR
	diff_col = colour;
	#endif
	
	// Specular Map
	#ifdef BRANCH_SPEC_MAP
	spec_col = spec_col * (texture(m_tex_specular, vec2(uvw.x, uvw.y))).x; 
	#endif
	
	// Bump Mapping
	#ifdef BRANCH_BUMP_MAP
	float eps = 0.01; 
	float r_eps = (1.0 / eps) * 2.f; 
	float bump_u = texture(m_tex_bump, vec2(uvw.x + eps, uvw.y)).x - texture(m_tex_bump, vec2(uvw.x - eps, uvw.y)).x; 
	float bump_v = texture(m_tex_bump, vec2(uvw.x, uvw.y + eps)).x - texture(m_tex_bump, vec2(uvw.x, uvw.y - eps)).x; 
	vec3 bump = vec3(bump_u * r_eps, bump_v * r_eps, 0.0); 
	normal = normalize(normal_world + (bump * m_coeff_bump * 0.01));
	//frag_colour = vec4(normal, 1.0); 
	//return; 
	//frag_colour = vec4(bump, 1.0); 
	//return; 
	#endif
	
	// Blinn_Phong BRDF (Sun only) (WS calculations)	
	// Sun Lighting Vectors
	vec3 light_dir    = s_direction; 
	vec3 view_dir     = normalize(cam_pos_world - pos_world);
	vec3 half_v       = normalize(light_dir + view_dir);
	
	// Basic Lambertian diffuse term
	//float lambertian = max(dot(normal, light_dir), 0.0);
	
	float lambertian = max(dot(normal_world, light_dir), 0.0);
	
	// Blinn Phong Specular Term
	float bpspec = pow(max(dot(normal, half_v), 0.0), m_exp_gloss);
	//vec3 refl_dir = reflect(-light_dir, normal);
    //float pspec = max(dot(refl_dir, view_dir), 0.0);
		
	// Shading Terms
	vec3 ambient  = amb_col * m_coeff_ambient;
	vec3 diffuse  = lambertian * diff_col * sun_strength * s_colour * m_coeff_diffuse; 
	vec3 specular = bpspec * spec_col * s_colour * s_strength * m_coeff_specular;  
	//vec3 specular = pspec * spec_col * sun_strength * m_coeff_specular;  

	// Linear output
	vec3 out_c = ambient + diffuse + specular; 
	
	// Gamma correction
	out_c = pow(out_c, vec3(1.0 / 2.2)); 
	
	// Output
	frag_colour = vec4(out_c, 1.0);  
	//frag_colour = vec4(normal, 1.0);  
}
