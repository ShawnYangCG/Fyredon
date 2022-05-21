// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	GroundPlane.cpp: : Niall
=============================================================================*/
#include "Fyredon/Other/pch.h"

// Implements
#include "Fyredon/Render/GroundPlane.h"
#include "Fyredon/Render/Texture.h"
#include "Fyredon/Render/Material.h"

// Extern
#include "glm/common.hpp"

#include <filesystem>

// =================== Ground Plane Implementation ===================

Fyredon::GroundPlane::GroundPlane()
	: Mesh_OBJ("Ground_Plane", "..//Assets//Mesh//grid.obj")
{
	size = 1.f, tile = 2.f; 
	load_obj();

	// Override Default Mesh Uber Shader
	set_shader("..//Shaders//mesh_vert_common.glsl", "..//Shaders//ground_fragment.glsl");

	material->set_tex_diffuse("..//Assets//Images//Misc//grid_RGB.png");
	material->build_mesh_shader();

	// Store Orginal Grid Mesh Data
	org.resize(vert_data.size());
	for (std::size_t v = 0; v < vert_data.size(); ++v)
	{
		org[v] = vert_data[v];
	}
}

void Fyredon::GroundPlane::set_size(float Size)
{
	// Reset to org size
	for (std::size_t v = 0; v < vert_data.size(); ++v) vert_data[v].pos = org[v].pos;

	// Update Mesh with scaled Position
	size = Size; 
	for (Vert &v : vert_data)
	{
		v.pos *= size; 
	}
	update_data_mesh(vert_data);
}

void Fyredon::GroundPlane::set_tile(float Tile)
{
	// Reset to org uv
	for (std::size_t v = 0; v < vert_data.size(); ++v) vert_data[v].uv = org[v].uv;

	// Update Mesh with scaled UV's.
	tile = Tile;
	for (Vert &v : vert_data)
	{
		v.uv *= tile;
	}
	update_data_mesh(vert_data);
}