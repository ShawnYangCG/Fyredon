// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	GroundPlane.cpp: : Niall
=============================================================================*/
#include "Fyredon/Other/pch.h"

// Implements
#include "Fyredon/Render/GroundPlane.h"
#include "Fyredon/Render/Texture.h"

// Extern
#include "glm/common.hpp"

#include <filesystem>

// =================== Ground Plane Implementation ===================

Fyredon::GroundPlane::GroundPlane()
	: Mesh_OBJ("Ground_Plane", "..//Assets//Mesh//grid.obj")
{
	size = 1.f, tile = 2.f; 
	load_obj();
	set_shader("..//Shaders//ground.vert", "..//Shaders//ground.frag");
	load_texture("..//Assets//Images//Misc//grid_RGB.png", 0);
	tex->set_params(Texture::filter_type::LINEAR);
	set_colour(glm::vec3(1.f, 0.f, 0.f));
	mode = Render_Mode::RENDER_MESH;
}

void Fyredon::GroundPlane::set_size(float Size)
{
	size = Size; 
	for (Vert &v : vert_data)
	{
		v.pos *= size; 
	}
	// Update Mesh with scaled Position
	update_data_mesh(vert_data);
}

void Fyredon::GroundPlane::set_tile(float Tile)
{
	tile = Tile;
	for (Vert &v : vert_data)
	{
		v.uv *= tile;
	}
	// Update Mesh with scaled UV's.
	update_data_mesh(vert_data);
}