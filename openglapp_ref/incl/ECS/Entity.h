#pragma once
#include "ECS/Component.h"
#include <vector>
#include <string>
#include <map>
class Component;
// the class of entity, this class should be inherited by all object used in system
class Entity
{
private:
	std::string name;
	std::vector<Component*> components;
	std::map<std::string, int> componentList;

public:
	Entity();
	Entity(std::string name);
	void addComponent();
	void addComponent(std::string componentName);
	void addComponent(Component* component);
	void deleteComponent(int index);
	Component* getComponentByIndex(int index);
	std::string getName();
	void setName(std::string name);
	Component* getComponentByName(std::string componentName);
	void render();

	// if you want to add some commonly used component you can call the functions bellow
	// or you can add your own component when you inherit the entity class
	void addTransformComponent();
};

