// Copyright Fyredon. All Rights Reserved.
/*=============================================================================
	World.h: ECS Game World : Shawn, Niall
=============================================================================*/
#pragma once

#include "Fyredon/ECS/Entity.h"
#include "Fyredon/GUI/GUI_PanelOrgnizer.h"

class World
{
public:
	~World();

	static World& get_world();

	World(GUI_PanelOrgnizer* gui_org);
	void addEntity(Entity* entity);
	void addCamera(Entity* entity);
	void init();
	void tick();
	void reset();
	void stop();

	Entity* getEntityByIndex(int index);
	Entity* getEntityByName(std::string EntityName);
	Entity* getCameraByIndex(int index);
	Entity* getCameraByName(std::string CameraName);

public:
	std::vector<Entity*> entities;
	std::vector<Entity*> cameras;

	std::map<std::string, int> entityList;
	std::map<std::string, int> cameraList;

	GUI_PanelOrgnizer* gui_org;

private:
	static World world; 
	World();
};

