// Copyright Fyredon. All Rights Reserved.
/*=============================================================================
	Camera_Entity.h: ECS Entity with root Camera_Component: Shawn, Niall
=============================================================================*/
#pragma once

#include "Fyredon/ECS/Entity.h"
#include "Fyredon/ECS/camera_component.h"

namespace Fyredon {

class camera_entity : public Entity
{
public:
	camera_entity();

	camera_component* get_camera_component(); 
};

}

