#pragma once
#include "ext/dearimgui/imgui.h"
#include <string>
class Component_GUI;
class Component
{
public:
	std::string componentName;
	// when you create your own component you need to inherit this class and 
	// add your own variable here
	// == variable ==

	// ==============
	Component();
	Component(std::string componentName);
	void renderGUI();
	// rewrite this function to create your own component GUI
	virtual void GUI_struct();

};

