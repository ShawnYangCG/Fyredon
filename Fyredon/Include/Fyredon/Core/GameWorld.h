// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	GameWorld.h: (Pre-ECS) Container to hold state of data in game : Niall
=============================================================================*/
#pragma once

#include <unordered_map>
#include <string>
#include <vector>

#include "Fyredon/Render/Mesh.h"
#include "Fyredon/Render/Light.h"
#include "Fyredon/Terrain/TerrainGenerator.h"

namespace Fyredon {

class Renderer;

class GameWorld final 
{
public:
	~GameWorld() = default;

	static GameWorld& get_world();

	void tick(float delta_time);

	void create_mesh_obj(const char* name, const char* file_path);
	//void create_mesh(const char* name, const char* file_path, const char *vert_shader, const char *frag_shader);
	void create_light(const char* name, glm::vec3 Pos, float Strength); 
	void create_terrain();


	void delete_mesh(const char* name); 
	void delete_light(const char* name);

	TerrainGenerator* terrain_generator;
	bool terrain_loaded = false;

private:
	GameWorld();

	// Ownership of game world entities
	std::vector<Mesh>      prims;
	std::vector<Light>     lights; 

	// Maps for named access
	std::unordered_map<std::string, Mesh*>  prims_map; 
	std::unordered_map<std::string, Light*> lights_map; 

	// Game time
	double total_game_time;

	static GameWorld world;

	friend class GUI_GameWorld;
};


}
