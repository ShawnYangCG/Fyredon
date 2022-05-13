#pragma once
#include "Fyredon/ECS/World.h"
#include <ctime>
#include "Fyredon/ECS/test_move_component.h"
#include "Fyredon/ECS/navigation_component.h"
#include "Fyredon/ECS/System/EngineRenderSystem.h"
class World;
class EngineRenderSystem;
class NavigationSystem
{
public:
	World* world = nullptr;
	EngineRenderSystem* enrenSys = nullptr;
	clock_t lastTime;
	std::vector<Entity*> entities;
	Fyredon::Mesh* wall = nullptr;
	Entity* destPoint = nullptr;
	glm::vec3 targetPosition;
	Entity* map = nullptr;
	float deltaTime = 0.04;	// second
	NavigationSystem(World* world);
	void tick();
	void addEntity(Entity* entity);
	glm::vec3 getTargetPosition();
	bool isDestinationChange(glm::vec3 destPosition);
	bool isStepIntoGrid(glm::vec2 src, glm::vec2 grid);
	void printMap(list<glm::vec2> path);
	void randerWalls();
	void gotoxy(int x,int y);
};

