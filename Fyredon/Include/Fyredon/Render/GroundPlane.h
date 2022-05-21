// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	GroundPlane.h: :Niall
=============================================================================*/
#pragma once

// Project Headers
#include "Fyredon/Render/Mesh_OBJ.h"

#include "Fyredon/Render/Texture.h"

namespace Fyredon
{
class GroundPlane : public Mesh_OBJ
{
public:
	GroundPlane();
	~GroundPlane();

	void set_size(float Size);
	void set_tile(float Tile);

	//TextureImage* tex; 

public:
	float size, tile;
};

}

