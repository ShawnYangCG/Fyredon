#pragma once
#include "Fyredon/ECS/Component.h"
#include "Fyredon/ECS/fbx_entity.h"
#include "Fyredon/FBX/fbx_mesh.h"
#include <time.h>
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
	int currentFrameIndex = 0;
	// ==============
	
	// 4.
	// rewrite this function to create your own component GUI
	virtual void GUI_struct() override;
	virtual void tick() override;
	virtual void play() override;
	virtual void stop() override;
	virtual void reset() override;
	void playAnimation();
	void checkValid();
	void setPlay();
private:
	time_t timer_start;
	time_t timer_current;
	time_t pause_time_start;
	time_t pause_time_end;
	time_t pause_time = 0;
	int secondPerFrame = 50;
	bool isPlay = false;
	bool isReset = true;
};
// 5.
// don't forget to add this header in ECS.h

