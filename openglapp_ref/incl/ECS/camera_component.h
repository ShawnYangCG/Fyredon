#pragma once
#include "ECS/Component.h"
#include "camera.h"
class Camera;
// 1.
// create your own component by inherit Component class
class camera_component : public Component
{
public:
	// 2. 
	// rewrite Constructor (give a name to this component)
	camera_component(Camera* camera);
	// 3.
	// when you create your own component you need to inherit this class and 
	// add your own variable here
	// == variable ==
	Camera* camera;
	// ==============

	// 4.
	// rewrite this function to create your own component GUI
	virtual void GUI_struct() override;
	// virtual void tick() override;
	// virtual void render() override;
};
// 5.
// don't forget to add this header in ECS.h

