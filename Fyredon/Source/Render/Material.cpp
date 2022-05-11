// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	Material.cpp: Material System : Niall
=============================================================================*/
#include "Fyredon/Other/pch.h"
// Implements
#include "Fyredon/Render/Material.h"

// Project Includes
#include "Fyredon/Render/Renderer.h"
#include "Fyredon/Render/Mesh.h"
#include "Fyredon/Render/Shader.h"

#include "GLEW/glew.h"

#include <string>
#include <vector>

Fyredon::Material::Material(Mesh& ParentMesh)
	: parent_mesh(ParentMesh), tex_diffuse(nullptr), tex_specular(nullptr)
{
	parms.use_diff_map = false, parms.use_spec_map = false, parms.use_vert_col = false; 

	parms.coeff_ambient  = 1.f;
	parms.coeff_diffuse  = 1.f;
	parms.coeff_specular = 1.f; 
	parms.exp_gloss      = 4.f; 

	parms.colour_ambient = glm::vec3(0.2f);
	parms.colour_diffuse = glm::vec3(0.3f, 0.5f, 0.2f);
	parms.colour_specular = glm::vec3(1.f); 

	parms_changed = false; 
}

// Info asssume textures are owned by material class. 
Fyredon::Material::~Material()
{
	if (tex_diffuse)  delete tex_diffuse; 
	if (tex_specular) delete tex_specular; 
}

// Info : Host side branching to avoid GPU side fragment branching (and thus execution divergence)
void Fyredon::Material::build_mesh_shader()
{
	// Set fragment shader preproc defines based on material params state
	std::vector<const char*> frag_preproc; 

	if (parms.use_diff_map) frag_preproc.push_back("#define BRANCH_DIFF_MAP");
	if (parms.use_spec_map) frag_preproc.push_back("#define BRANCH_SPEC_MAP");
	if (parms.use_vert_col) frag_preproc.push_back("#define BRANCH_DIFF_VERTCOLOUR");

	// Also check if lighting, shadows enabled in renderer class
	Renderer& renderer = Renderer::get_renderer();
	if (renderer.parms.enable_sun) frag_preproc.push_back("#define BRANCH_LIGHT_SUN");

	// Rebuild shader with new fragment preproc defines. 
	parent_mesh.shader.build(frag_preproc);
}

void Fyredon::Material::set_tex_diffuse(const char* filepath)
{
	if (tex_diffuse) delete tex_diffuse; 

	std::string nm = parent_mesh.name + " Diffuse_Texture"; 
	tex_diffuse = new TextureImage(filepath, nm.c_str(), 0, true);
	tex_diffuse->load();
	parent_mesh.shader.set_int("textures.tex_diffuse", 0);
}

void Fyredon::Material::set_tex_specular(const char* filepath)
{
	if (tex_specular) delete tex_specular;

	std::string nm = parent_mesh.name + " Specular_Texture";
	tex_specular = new TextureImage(filepath, nm.c_str(), 0, true);
	tex_specular->load();
	parent_mesh.shader.set_int("textures.tex_specular", 1);
}

// Info : To be called by parent Mesh::set_shader_uniforms()
void Fyredon::Material::set_uniforms()
{
	Shader& shad = parent_mesh.shader;

	// Term Coeffs
	shad.set_float("mat_parms.coeff_ambient",  parms.coeff_ambient);
	shad.set_float("mat_parms.coeff_diffuse",  parms.coeff_diffuse);
	shad.set_float("mat_parms.coeff_specular", parms.coeff_specular);
	shad.set_float("mat_parms.exp_gloss",      parms.exp_gloss);

	// Term Colours
	shad.set_vec3("mat_parms.colour_ambient",  parms.colour_ambient);
	shad.set_vec3("mat_parms.colour_diffuse",  parms.colour_diffuse);
	shad.set_vec3("mat_parms.colour_specular", parms.colour_specular);

	// Reset Texture Sampler Uniforms
	shad.set_int("tex_diffuse",  0);  // Unit_0
	shad.set_int("tex_specular", 1);  // Unit_1
}

// Info : Activate Bind textures used for maps (only if textures are allocated)
void Fyredon::Material::activate_bind_textures()
{
	if (tex_diffuse)
	{
		glActiveTexture(GL_TEXTURE0);
		tex_diffuse->bind();
	}
	if (tex_specular)
	{
		glActiveTexture(GL_TEXTURE1);
		tex_specular->bind();
	}
}