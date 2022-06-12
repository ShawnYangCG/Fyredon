#pragma once
#include "Fyredon/ECS/Component.h"
#include "glm/glm.hpp"
// 1.
// create your own component by inherit Component class
class transform_component : public Component
{
public:
	// 2. 
	// rewrite Constructor (give a name to this component)
	transform_component();
	// 3.
	// when you create your own component you need to inherit this class and 
	// add your own variable here
	// == variable ==
	float matrixTranslation[3] = { 0,0,0 }, matrixRotation[3] = { 0,0,0 }, matrixScale[3] = { 1,1,1 };
	float matrixTranslation_check[3] = { 0,0,0 }, matrixRotation_check[3] = { 0,0,0 }, matrixScale_check[3] = { 1,1,1 };
	bool isChanged = false;
	// ==============
	
	// 4.
	// rewrite this function to create your own component GUI
	virtual void GUI_struct() override;
	virtual void tick() override;
	virtual void reset() override;
	void checkChanged();
	// virtual void tick() override;
	// virtual void render() override;
};
// 5.
// don't forget to add this header in ECS.h

