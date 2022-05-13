// Copyright Fyredon. All Rights Reserved.
/*=============================================================================
	Terrain_Entity.cpp: ECS Entity with root Terrain_Component: Niall, Izzy
=============================================================================*/
#include "Fyredon/Other/pch.h"
// Implements
#include "Fyredon/ECS/terrain_entity.h"

// Project Includes
#include "Fyredon/ECS/terrain_component.h"
#include "Fyredon/ECS/World.h"
//#include "Fyredon/ECS/transform_component.h"

Fyredon::terrain_entity::terrain_entity()
{
	// Root Component is Terrain_Component
	addComponent(new terrain_component(this));
	// Default entitiy name.
	this->setName(std::string("Terrain_") + std::to_string(World::get_world().counts.terrain_entity_count++));

	// Transform Component atop
	//this->addTransformComponent();
}

Fyredon::terrain_component* Fyredon::terrain_entity::get_terrain_component()
{
	// Assume Terrain is 0th (root) component
	return static_cast<terrain_component*>(getComponentByIndex(0));
}
