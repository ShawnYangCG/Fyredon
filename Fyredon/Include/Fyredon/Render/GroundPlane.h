// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	GroundPlane.h: :Niall
=============================================================================*/
#pragma once

// Project Headers
#include "Fyredon/Render/Mesh.h"

namespace Fyredon
{
class GroundPlane : public Mesh
{
public:
	GroundPlane();
	~GroundPlane() = default;

	void set_size(float Size);
	void set_tile(float Tile);

public:
	float size, tile;
};

}

