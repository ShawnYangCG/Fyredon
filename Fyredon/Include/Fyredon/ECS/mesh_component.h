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

private:
	void mesh_changed();
	std::size_t ren_idx; 

public:

	struct {
		// Mesh parms
		const char* file_path; 
		const char* diff_path;
		const char* spec_path;

		bool use_diff = false;
		bool use_spec = false; 

		// Other Material parms (expose from inner mesh directly) 
	} parms;

	struct {
		glm::vec3 min, max; 
	} bounds;

	Fyredon::Mesh_OBJ* mesh;
};
