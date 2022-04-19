#pragma once
#include "Fyredon/ECS/Component.h"
#include "Fyredon/ECS/transform_component.h"
class test_move_component : public Component
{
public:
	// 2. 
	// rewrite Constructor (give a name to this component)
	test_move_component();
	// 3.
	// when you create your own component you need to inherit this class and 
	// add your own variable here
	// == variable ==
	float speed = 1.5;
	glm::vec3 front = { 0,0,1 };
	// ==============
	bool moveOneStep(glm::vec3 dest, float deltaTime);
	glm::vec3 getCurrentPosition();
	void setPosition(glm::vec3 pos);
	void setRotation(float angle);
	// 4.
	// rewrite this function to create your own component GUI
	virtual void GUI_struct() override;
	//virtual void tick() override;
	
	// virtual void render() override;
};
// 5.
// don't forget to add this header in ECS.h

