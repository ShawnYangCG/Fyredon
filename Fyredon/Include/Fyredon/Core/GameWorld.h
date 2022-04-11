// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	GameWorld.h: Temporary (Pre-ECS) Container to whole state of data in game : Niall
=============================================================================*/
#pragma once

// Std
#include <unordered_map>
#include <string>
#include <vector>

#include "Fyredon/Render/Primitive.h"
#include "Fyredon/Render/Light.h"

namespace Fyredon {

class Renderer;

class GameWorld final 
{
public:
	~GameWorld() = default;

	static GameWorld& get_world();

	void tick(float delta_time);

	void create_mesh(const char* name, const char* file_path);
	void create_mesh(const char* name, const char* file_path, const char *vert_shader, const char *frag_shader);
	void create_light(const char* name, glm::vec3 Pos, float Strength); 

	void delete_mesh(const char* name); 
	void delete_light(const char* name);
private:
	GameWorld();

	// Ownership of game world entities
	std::vector<Primitive> prims;
	std::vector<Light>     lights; 

	// Maps for named access
	std::unordered_map<std::string, Primitive*> prims_map; 
	std::unordered_map<std::string, Light*>     lights_map; 

	// Game time
	double total_game_time;

	static GameWorld world;

	friend class GUI_GameWorld;
};


}
