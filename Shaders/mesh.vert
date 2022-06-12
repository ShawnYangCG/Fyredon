// Copyright Fyredon. All Rights Reserved.
/*=============================================================================
	mesh.vert : Mesh Vertex Shader : Niall
=============================================================================*/

#version 400 core 

// Input 
layout (location = 0) in vec3 v_P;
layout (location = 1) in vec3 v_N;
layout (location = 2) in vec3 v_C;
layout (location = 3) in vec2 v_UV;

// Output
out vec3 colour; 
out vec3 normal;
out vec3 pos_world;
out vec3 normal_world;

// Uniforms
uniform mat4 model; 
uniform mat4 view; 
uniform mat4 proj; 
uniform mat3 model_normal; 

void main()
{
	// Forward As Is
	colour = v_C; 
	normal = v_N;

	// World Space transforms
	pos_world    = vec3(model * vec4(v_P, 1.0));
	normal_world = model_normal * v_N; 

	// Pos
	gl_Position = proj * view * model * vec4(v_P, 1.0); 
}

