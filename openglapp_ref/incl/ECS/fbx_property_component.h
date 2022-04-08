#pragma once
#include"ECS/ECS.h"
// 1.
// create your own component by inherit Component class
class fbx_property_component:public Component
{
public:
	// 2. 
	// rewrite Constructor (give a name to this component)
	fbx_property_component();
	fbx_property_component(float startTime, float endTime, float speed);

	// 3.
	// when you create your own component you need to inherit this class and 
	// add your own variable here
	// == variable ==
	float speed = 0;
	float startTime = 0;
	float endTime = 0;
	float currentFrameIndex = 0;
	// ==============
	
	// 4.
	// rewrite this function to create your own component GUI
	virtual void GUI_struct() override;
};
// 5.
// don't forget to add this header in ECS.h

