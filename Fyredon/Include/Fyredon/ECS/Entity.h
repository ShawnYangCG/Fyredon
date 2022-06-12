// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	Entity.h : Base Entity class, Shawn. 
=============================================================================*/

#pragma once
#include "Fyredon/ECS/Component.h"
#include <vector>
#include <string>
#include <map>
class Component;
// the class of entity, this class should be inherited by all object used in system
class Entity
{
public:
	std::vector<Entity*> childs;
private:
	std::string name;
	std::vector<Component*> components;
	Entity* parent = nullptr;
	std::map<std::string, int> componentList;

public:
	Entity();
	Entity(std::string name);
	~Entity();

	virtual void render();
	virtual void tick();
	virtual void play();
	virtual void init();
	virtual void stop();
	void reset();

	void addComponent();
	void addEntity(Entity* entity);
	void addComponent(std::string componentName);
	void addComponent(Component* component);
	void deleteComponent(int index);

	Component* getComponentByIndex(int index);
	std::string getName();
	void setName(std::string name);
	Component* getComponentByName(std::string componentName);

	// if you want to add some commonly used component you can call the functions bellow
	// or you can add your own component when you inherit the entity class
	void addTransformComponent();
};

