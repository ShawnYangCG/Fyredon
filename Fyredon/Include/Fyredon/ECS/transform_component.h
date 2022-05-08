#pragma once
#include "Fyredon/ECS/Component.h"
#include "glm/glm.hpp"

class transform_component : public Component
{
public:

	transform_component();

	virtual void GUI_struct() override;
	virtual void tick() override;

public:
	float matrix_translation[3];
	float matrix_rotation   [3]; 
	float matrix_scale      [3];

	bool isChanged;
};

