// Copyright Fyredon. All Rights Reserved.
/*=============================================================================
	mesh_vert_common.glsl : Common vertex shader for all meshes
=============================================================================*/
#version 450 core 

// ======= Input ======= 
layout (location = 0) in vec3 v_P;
layout (location = 1) in vec3 v_N;
layout (location = 2) in vec3 v_C;
layout (location = 3) in vec3 v_UV;

// ======= Output ======= 
out vec3 colour; 
out vec3 pos_world;
out vec3 normal_world; 
out vec3 uvw;

// ======= Uniforms ======= 
uniform mat4 model; 
uniform mat4 view; 
uniform mat4 proj; 
uniform mat3 model_normal; 

void main()
{
	// Forward only
	colour = v_C; 
	uvw = v_UV; 

	// World Space transforms
	pos_world    = vec3(model * vec4(v_P, 1.0));
	normal_world = model_normal * v_N; 

	// Pos output
	gl_Position = proj * view * model * vec4(v_P, 1.0); 
}

