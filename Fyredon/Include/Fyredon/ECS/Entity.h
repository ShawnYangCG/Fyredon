// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	Entity.h : Base Entity class, Shawn. 
=============================================================================*/
#pragma once

#include <vector>
#include <string>
#include <map>

#include "Fyredon/ECS/Component.h"

class Component;
// the class of entity, this class should be inherited by all object used in system
class Entity
{
public:
	Entity();
	Entity(std::string name);

	virtual void tick();

	virtual void init();

	virtual void stop();

	void reset();

	//void addComponent();

	//void addComponent(std::string componentName);

	void addComponent(Component* component);

	void addEntity(Entity* entity);

	void deleteComponent(int index);

	Component* getComponentByIndex(int index);

	std::string getName();

	void setName(std::string name);

	Component* getComponentByName(std::string componentName);

	// if you want to add some commonly used component you can call the functions bellow
	// or you can add your own component when you inherit the entity class
	void addTransformComponent();

	std::vector<Entity*> childs;
private:
	Entity* parent = nullptr;

	std::vector<Component*> components;
	std::map<std::string, int> componentList;

	std::string name;
};

