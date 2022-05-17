#pragma once
#include "Fyredon/ECS/World.h"
#include <ctime>
#include "Fyredon/ECS/test_move_component.h"
#include "Fyredon/ECS/navigation_component.h"

class NavigationSystem
{
public:
	clock_t lastTime;
	std::vector<Entity*> entities;
	//Fyredon::Mesh_OBJ* wall;
	Entity* destPoint;
	glm::vec3 targetPosition;
	Entity* map;
	float deltaTime = 0.04;	// second
	NavigationSystem();
	void tick();
	void addEntity(Entity* entity);
	glm::vec3 getTargetPosition();
	bool isDestinationChange(glm::vec3 destPosition);
	bool isStepIntoGrid(glm::vec2 src, glm::vec2 grid);
	void printMap(list<glm::vec2> path);
	void gotoxy(int x,int y);
};

