#pragma once
#include "Fyredon/ECS/Entity.h"
#include "Fyredon/ECS/mesh_component.h"
// 1.
// include your own header file
#include "Fyredon/Terrain/TerrainGenerator.h"
// 2.
// inherit the entity class
class terrain_entity : public Entity
{
public:
	// 3.
	// add your own veriable here
	TerrainGenerator* terrain_generator;

	std::string fileName = "";
	int res = 512;
	bool flat_normals = false;


	// 4.
	// rewrite the constructor
	terrain_entity();
	void redrawMesh();
};
// 5.
// don't forget to add this header in ECS.h
