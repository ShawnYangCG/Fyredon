// Copyright Fyredon. All Rights Reserved.
/*=============================================================================
	Terrain_Component.h: ECS Camera Component : Niall, Isiah
=============================================================================*/
#pragma once

#include "Fyredon/ECS/Component.h"

#include "Fyredon/Terrain/TerrainGenerator.h"
#include "Fyredon/Render/Mesh.h"
#include "Fyredon/Render/Renderer.h"

// Info : This forms the base component of the Camera_Entity class.
class Entity;
namespace Fyredon {

class terrain_component : public Component
{
public:
	terrain_component(Entity* parent);
	virtual ~terrain_component();

	virtual void GUI_struct() override;

	virtual void tick() override;

public:
	// Internally contains mesh and material to expose to GUI.
	TerrainGenerator *terrain_generator;

	struct
	{
		const char* height_filepath;
		const char* texture_filepath;
		int32_t res_sqr; 
	} parms;
};

}

