// Copyright Fyredon. All Rights Reserved.
/*=============================================================================
	Shaderball_Entity.cpp: ECS Mesh (Shaderball) Entity : Niall
=============================================================================*/
#include "Fyredon/Other/pch.h"
// Implements
#include "Fyredon/ECS/shaderball_entity.h"

#include "Fyredon/ECS/World.h"
#include "Fyredon/ECS/mesh_component.h"

Fyredon::shaderball_entity::shaderball_entity()
{
	// Root Component is Mesh_Component
	this->addComponent(new mesh_component(this));
	// Default entitiy name.
	this->setName(std::string("Shaderball_") + std::to_string(World::get_world().counts.shaderball_entity_count++));
}

mesh_component* Fyredon::shaderball_entity::get_mesh_component()
{
	// Assume mesh is 0th component
	return static_cast<mesh_component*>(getComponentByIndex(0));
}