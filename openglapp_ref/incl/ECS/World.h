#pragma once
#include "ECS/Entity.h"
#include <iostream>
class Entity;
class World
{
public:
	std::vector<Entity*> entities;
	std::map<std::string, int> entityList;
	int selectedEntityIndex = -1;

	World();
	void addEntity(Entity* entity);
	void init();
	void tick();
	void reset();
	void stop();
	Entity* getEntityByIndex(int index);
	Entity* getEntityByName(std::string EntityName);
};

