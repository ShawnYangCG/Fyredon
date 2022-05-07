#pragma once
#include "Fyredon/ECS/Component.h"
#include "Fyredon/Render/Camera.h"
#include "Fyredon/Render/Mesh.h"
#include "Fyredon/Render/Renderer.h"

class camera_component : public Component
{
public:
	camera_component(Fyredon::Camera* camera);

	Fyredon::Camera* camera;

	Fyredon::Mesh* guide_mesh;

	virtual void GUI_struct() override;
	// virtual void tick() override;
	// virtual void render() override;
};


