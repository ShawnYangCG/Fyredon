#pragma once
#include "dearimgui/imgui.h"
#include "Fyredon/ECS/Entity.h"
#include <string>
class Entity;
class Component
{
public:
	Component();
	Component(std::string componentName, Entity* parent);
	virtual ~Component();

	virtual void init();

	virtual void tick();

	virtual void stop();

	void reset();

	void renderGUI();

	void setRunning(bool isRunning);

	// rewrite this function to create your own component GUI
	virtual void GUI_struct();

public:
	std::string componentName;
	Entity* parent = nullptr;
	bool isActive = true;

private:
	// you need to put value into this truct when initialize
	Component* componentBackup = nullptr;

protected:
	int memmory_size;
	bool isRunning = false;
};

