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
	std::vector<Entity*> cameras;
	std::map<std::string, int> cameraList;
	GUI_PanelOrgnizer* gui_org;

	World(GUI_PanelOrgnizer* gui_org);
	~World();
	void addEntity(Entity* entity);
	void addCamera(Entity* entity);
	void init();
	void tick();
	void play();
	void reset();
	void stop();
	Entity* getEntityByIndex(int index);
	Entity* getEntityByName(std::string EntityName);
	Entity* getCameraByIndex(int index);
	Entity* getCameraByName(std::string CameraName);
	bool isPlay();
	bool isStop();
	bool isReset();
private:
	bool is_play = false;
	bool is_stop = true;
	bool is_reset = false;
};

