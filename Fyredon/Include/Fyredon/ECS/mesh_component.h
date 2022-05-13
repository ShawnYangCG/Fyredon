// Copyright Fyredon. All Rights Reserved.
/*=============================================================================
	Mesh_Component.h: ECS Mesh Component : Niall, Shawn
=============================================================================*/
#pragma once

#include "glm/glm.hpp"

#include "Fyredon/ECS/Component.h"
#include "Fyredon/Render/Mesh.h"
#include "Fyredon/Render/Mesh_OBJ.h"
#include "Fyredon/Render/Renderer.h"

class mesh_component : public Component
{
public:

	mesh_component(Entity *parent);
	virtual ~mesh_component();

	virtual void GUI_struct() override;

	virtual void tick() override;

	// Material
	// get shaderball thumbnail
	// get diffuse map  thumbnail
	// get specular map thumbnail

	// Ref material parms in mesh...

public:

	struct {
		// Mesh parms
		const char* file_path; 

		// Material parms
	} parms;

	struct {
		glm::vec3 min, max; 
	} bounds;

	Fyredon::Mesh_OBJ* mesh;
};
