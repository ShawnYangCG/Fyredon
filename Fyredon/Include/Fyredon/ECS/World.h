#pragma once
#include "Fyredon/ECS/Entity.h"
#include "Fyredon/GUI/GUI_PanelOrgnizer.h"
#include <iostream>
class Entity;
class World
{
public:
	std::vector<Entity*> entities;
	std::map<std::string, int> entityList;
	GUI_PanelOrgnizer* gui_org;

	World(GUI_PanelOrgnizer* gui_org);
	void addEntity(Entity* entity);
	void init();
	void tick();
	void reset();
	void stop();
	Entity* getEntityByIndex(int index);
	Entity* getEntityByName(std::string EntityName);
};

