#pragma once
#include "Fyredon/ECS/Entity.h"
#include "Fyredon/ECS/Component.h"
#include "Fyredon/ECS/mesh_component.h"
#include "Fyredon/ECS/fbx_property_component.h"

// 1.
// include your own header file
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
	fbx_entity(std::string name, std::string filePath, float startTime, float endTime, float timeStep);
};
// 5.
// don't forget to add this header in ECS.h
