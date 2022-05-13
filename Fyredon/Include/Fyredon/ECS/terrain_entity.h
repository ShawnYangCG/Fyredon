// Copyright Fyredon. All Rights Reserved.
/*=============================================================================
	Terrain_Entity.h: ECS Entity with root Terrain_Component: Niall, Izzy
=============================================================================*/
#pragma once

#include "Fyredon/ECS/Entity.h"
#include "Fyredon/ECS/terrain_component.h"

namespace Fyredon {
class terrain_component; 

class terrain_entity : public Entity
{
public:
	terrain_entity();

	terrain_component* get_terrain_component();
};
}
