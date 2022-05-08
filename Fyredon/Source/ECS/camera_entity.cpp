// Copyright Fyredon. All Rights Reserved.
/*=============================================================================
	Camera_Entity.cpp: ECS Entity with root Camera_Component: Shawn, Niall
=============================================================================*/
#include "Fyredon/Other/pch.h"
// Implements
#include "Fyredon/ECS/camera_entity.h"

// Project Includes
#include "Fyredon/Render/Camera.h"
#include "Fyredon/ECS/camera_component.h"

Fyredon::camera_entity::camera_entity()
{
	// Root Component is Camera_Component
	this->addComponent(new camera_component());
	this->setName("Camera_" + this->getComponentByIndex(0)->componentName);
}
