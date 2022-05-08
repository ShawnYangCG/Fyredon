// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	Material.h: Basic Material System, host side class : Niall
=============================================================================*/
#pragma once

#include "Fyredon/Render/Texture.h"

#include "glm/glm.hpp"

// Info : Materials are to be used with Meshes only. The mesh retains ownership of the shader.
//        The mesh shader is reloaded with preprocessor defines based on active material parameters. 

namespace Fyredon {

class Mesh; 
class Shader; 

class Material
{
public:
	Material(Mesh &ParentMesh); 
	~Material(); 

	void set_tex_diffuse(const char*  filepath);
	void set_tex_specular(const char* filepath);

	void set_uniforms();
	void activate_bind_textures(); 

	struct
	{
		bool use_diff_map, use_spec_map;
		bool use_vert_col; 

		glm::vec3 colour_ambient, colour_diffuse, colour_specular; 
		float coeff_ambient, coeff_diffuse, coeff_specular;
		float exp_gloss; 
	} parms;

	void build_mesh_shader();
protected:

	bool parms_changed; 
	Mesh& parent_mesh; 

public:
	TextureImage *tex_diffuse, *tex_specular;
};

}
