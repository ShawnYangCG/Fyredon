#pragma once
#include "ext/dearimgui/imgui.h"
#include <string>
class Component_GUI;
class Component
{
public:
	std::string componentName;
	// when you create your own component you need to inherit this class
	
	// add your own variable here
	// == variable ==
	bool isActive = true;
	// ==============
private:
	// you need to put value into this truct when initialize
	Component* componentBackup;

protected:
	int memmory_size;
	bool isRunning = false;

public:
	Component();
	Component(std::string componentName);
	virtual void init();
	void reset();
	virtual void tick();
	virtual void render();
	virtual void stop();
	void renderGUI();
	void setRunning(bool isRunning);
	// rewrite this function to create your own component GUI
	virtual void GUI_struct();

};

