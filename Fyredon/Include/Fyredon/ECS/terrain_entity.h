#pragma once
#include "Fyredon/ECS/Entity.h"
#include "Fyredon/ECS/mesh_component.h"

#include "Fyredon/Terrain/TerrainGenerator.h"

class terrain_entity : public Entity
{
public:
	terrain_entity();

	TerrainGenerator* terrain_generator;
};

