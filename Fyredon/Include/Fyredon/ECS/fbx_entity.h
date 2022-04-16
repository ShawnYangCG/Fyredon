#pragma once
#include "Fyredon/ECS/ECS.h"
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
	

	// 4.
	// rewrite the constructor
	fbx_entity(std::string name, std::string filePath, float startTime, float endTime, float timeStep);
};
// 5.
// don't forget to add this header in ECS.h
