// Copyright Fyredon. All Rights Reserved.
/*=============================================================================
	Camera_Entity.cpp: ECS Entity with root Camera_Component: Shawn, Niall
=============================================================================*/
#include "Fyredon/Other/pch.h"
// Implements
#include "Fyredon/ECS/camera_entity.h"

// Project Includes
#include "Fyredon/ECS/camera_component.h"
#include "Fyredon/ECS/World.h"
#include "Fyredon/Render/Camera.h"


Fyredon::camera_entity::camera_entity()
{
	// Root Component is Camera_Component
	this->addComponent(new camera_component(this));
	// Default entitiy name.
	this->setName(std::string("Camera_") + std::to_string(World::get_world().counts.camera_entity_count++));
}

Fyredon::camera_component* Fyredon::camera_entity::get_camera_component() 
{
	// Assume Camera is 0th component
	return static_cast<camera_component*>(getComponentByIndex(0));
}