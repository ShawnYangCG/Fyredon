// Copyright Fyredon. All Rights Reserved.
/*=============================================================================
	Shaderball_Entity.h: ECS Mesh (Shaderball) Entity : Niall
=============================================================================*/
#pragma once

#include "Fyredon/ECS/Entity.h"
#include "Fyredon/ECS/mesh_component.h"

namespace Fyredon {

	class shaderball_entity : public Entity
	{
	public:
		shaderball_entity();

		mesh_component* get_mesh_component();
	};

}

