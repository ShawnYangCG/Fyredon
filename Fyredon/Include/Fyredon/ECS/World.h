#pragma once
#include "Fyredon/ECS/Entity.h"
#include "Fyredon/GUI/GUI_PanelOrgnizer.h"
#include <iostream>
class Entity;
class AnimationSystem;
class NavigationSystem;
class World
{
public:
	std::vector<Entity*> entities;
	std::map<std::string, int> entityList;
	std::vector<Entity*> cameras;
	std::map<std::string, int> cameraList;
	GUI_PanelOrgnizer* gui_org;
	AnimationSystem* aniSys;
	NavigationSystem* navSys;

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
};

