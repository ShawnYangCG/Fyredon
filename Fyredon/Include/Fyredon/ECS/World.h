// Copyright Fyredon. All Rights Reserved.
/*=============================================================================
	World.h: ECS Game World Singleton : Shawn, Niall
=============================================================================*/

// Info : Regardless of where entities are created, their ownership is transfered
//        to game world who then handles their memory lifetime. 
#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "Fyredon/ECS/Entity.h"
#include "Fyredon/GUI/GUI_PanelOrgnizer.h"

class World
{
public:
	~World();

	static World& get_world();

	void create_camera_entity();

	void create_terrain_entity();

	void create_shaderball_entity();

	void create_doll_entity();

	void create_empty_entity();

	void init();
	void tick(float Dt);
	void reset();
	void stop();

	Entity* getEntityByIndex(int index);
	Entity* getEntityByName(std::string EntityName);

	Entity* getCameraByIndex(int index);
	Entity* getCameraByName(std::string CameraName);

private:
	void addEntity(Entity* entity);

public:
	std::vector<Entity*> entities;
	std::vector<Entity*> cameras;

	std::map<std::string, int> entityList;
	std::map<std::string, int> cameraList;

	GUI_PanelOrgnizer* gui_org;

	struct
	{
		int8_t terrain_entity_count;
		int8_t mesh_count; 
		int8_t camera_count;
		int8_t camera_entity_count;
		int8_t light_count; 
	} counts;
	
	struct
	{
		double total_editor_time, total_game_time;
		std::size_t total_editor_ticks, total_game_ticks;
	} time;

private:
	static World world; 
	World();
};

