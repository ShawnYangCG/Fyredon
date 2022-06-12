#pragma once
#include "Fyredon/ECS/Entity.h"
// 1.
// include your own header file
#include "Fyredon/ECS/System/NavigationSystem.h"

// 2.
// inherit the entity class
class navigation_entity : public Entity
{
public:
	// 3.
	// add your own veriable here
	NavigationSystem* navigationSys;
	// 4.
	// rewrite the constructor
	navigation_entity();
	navigation_entity(NavigationSystem* navigationSys);
	virtual void play() override;
	void setMap(Entity* entity);
	void setDest(Entity* entity);
	void addMoveObject(Entity* entity);
};
// 5.
// don't forget to add this header in ECS.h
