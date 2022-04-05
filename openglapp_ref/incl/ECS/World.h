#pragma once
#include "ECS/ECS.h"
class World
{
public:
	std::vector<Entity*> entities;
	std::map<std::string, int> entityList;
	int selectedEntityIndex = -1;

	World();
	void addEntity(Entity* entity);
	Entity* getEntityByIndex(int index);
	Entity* getEntityByName(std::string EntityName);
};

