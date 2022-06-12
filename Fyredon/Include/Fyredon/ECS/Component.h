#pragma once
#include "dearimgui/imgui.h"
#include "Fyredon/ECS/Entity.h"
#include <string>
class Entity;
class Component
{
public:
	std::string componentName;
	// when you create your own component you need to inherit this class
	Entity* parent;
	bool isActive = true;
	// add your own variable here
	// == variable ==
	
	// ==============
private:
	// you need to put value into this truct when initialize
	void* componentBackup = nullptr;

protected:
	int memmory_size = 0;
	bool isRunning = false;

public:
	Component();
	Component(std::string componentName);
	~Component();
	virtual void init();
	virtual void reset();
	virtual void tick();
	virtual void play();
	virtual void render();
	virtual void stop();
	void renderGUI();
	void setRunning(bool isRunning);
	// rewrite this function to create your own component GUI
	virtual void GUI_struct();
	int getSize();

};

