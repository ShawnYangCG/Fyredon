#pragma once
#include "Fyredon/ECS/Entity.h"
// 1.
// include your own header file
#include "Fyredon/Render/Camera.h"
#include "Fyredon/ECS/camera_component.h"

// 2.
// inherit the entity class
class camera_entity : public Entity
{
public:
	camera_entity();

	camera_entity();
};
// 5.
// don't forget to add this header in ECS.h
