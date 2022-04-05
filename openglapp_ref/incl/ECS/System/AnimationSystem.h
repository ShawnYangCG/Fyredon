#pragma once
#include "ECS/fbx_entity.h"
class fbx_entity;
class AnimationSystem
{
public:
	std::vector<fbx_entity*> entities;
	std::map<std::string, int> entityList;
	float currentFrameIndex = 0;

	AnimationSystem();
	void addFbxEntity(fbx_entity* entity);
	fbx_entity* getEntityByIndex(int index);
	fbx_entity* getEntityByName(std::string EntityName);
	void run();
};

