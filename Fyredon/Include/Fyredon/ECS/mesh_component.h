#pragma once

#include "glm/glm.hpp"

#include "Fyredon/ECS/Component.h"
#include "Fyredon/Render/Mesh.h"
#include "Fyredon/Render/Renderer.h"

class mesh_component : public Component
{
public:

	mesh_component(Fyredon::Mesh* mesh);
	~mesh_component();

	virtual void GUI_struct() override;
	virtual void tick() override;

	// Material
	// get shaderball thumbnail
	// get diffuse map  thumbnail
	// get specular map thumbnail

	// Ref material parms in mesh...

public:

	struct {
		glm::vec3 min, max; 
	} bounds;

	Fyredon::Mesh* mesh;
};
