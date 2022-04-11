#pragma once
#include "ECS/World.h"
#include <ctime>
#include "ECS/test_move_component.h"
#include "ECS/navigation_component.h"
#include "ECS/System/EngineRenderSystem.h"
class NavigationSystem
{
public:
	World* world;
	EngineRenderSystem* enrenSys;
	clock_t lastTime;
	std::vector<Entity*> entities;
	Mesh* wall;
	Entity* destPoint;
	Entity* map;
	float deltaTime = 0.04;	// second
	NavigationSystem(World* world);
	void tick();
	void addEntity(Entity* entity);
	void printMap(list<glm::vec2> path);
	void randerWalls();
	void gotoxy(int x,int y);
};

