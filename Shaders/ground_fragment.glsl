// Copyright Fyredon. All Rights Reserved.
/*=============================================================================
	ground_fragment.glsl : Fragment Shader for Ground Plane Grid : Niall
=============================================================================*/
//#version 450 core 

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
// Material Texture Maps
uniform sampler2D m_tex_diffuse; 

void main()
{
	vec3 col = vec3(uvw.x, uvw.y, 0.0);
	
	#ifdef BRANCH_DIFF_MAP
	// Sample Grid Texture
	vec4 tex_sample = texture(m_tex_diffuse, vec2(uvw.x, uvw.y));
	// Discard cells
	if (tex_sample.x < 0.05) discard;
	col = tex_sample.xyz; 
	#endif
	
	// Alpha by depth
	float a = col.x;
	a *= (3.5 - pos_proj.z);
	
	frag_colour = vec4(vec3(col), a);
}
