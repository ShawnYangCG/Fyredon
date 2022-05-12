// Copyright Fyredon. All Rights Reserved.
/*=============================================================================
	Camera_Component.h: ECS Camera Component : Niall, Shawn
=============================================================================*/

#pragma once
#include "Fyredon/ECS/Component.h"

#include "Fyredon/Render/Camera.h"
#include "Fyredon/Render/Mesh.h"
#include "Fyredon/Render/Renderer.h"

// Info : This forms the base component of the Camera_Entity class.
class Entity;
namespace Fyredon {

class camera_component : public Component
{
public:
	camera_component(Entity *parent);

	virtual void GUI_struct() override;

	virtual void tick() override;

	void set_camera_pos(const glm::vec3& pos);

public:
	Fyredon::Camera* camera;

	Fyredon::Mesh* guide_mesh;

	struct
	{
		float pos[3];
		float fov, aspect;
		bool free_look;
	} parms;
};
}

