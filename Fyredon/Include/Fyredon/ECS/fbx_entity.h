#pragma once
#include "Fyredon/ECS/Entity.h"
// 1.
// include your own header file
#include "Fyredon/ECS/mesh_component.h"
#include "Fyredon/ECS/fbx_property_component.h"
#include "Fyredon/FBX/fbx_mesh.h"
// 2.
// inherit the entity class
class fbx_entity : public Entity
{
public:
	// 3.
	// add your own veriable here
	fbx_mesh* FBXmesh;

	// 4.
	// rewrite the constructor
	fbx_entity(std::string name, std::string filePath);
	void init(const char* shader_vert, const char* shader_frag, float startTime = 0.f, float endTime = 0.f, float timeStep = 0.f);
};
// 5.
// don't forget to add this header in ECS.h
