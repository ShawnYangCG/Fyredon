#include "ECS/Entity.h"
#include "ECS/transform_component.h"
#include <iostream>
Entity::Entity()
{
    this->name = "new entity";
}

Entity::Entity(std::string name)
{
    this->name = name;
}

void Entity::addComponent()
{
    std::string newName = "new component " + std::to_string((this->components.size() + 1));
    if (this->componentList[newName] == NULL) {
        this->componentList[newName] = this->componentList.size();
        Component* newComponent = new Component(newName);
        this->components.push_back(newComponent);
    }
    else {
        std::cout << "Error: Entity [" << this->name << "] already contain component [" << newName << "]" << std::endl;
    } 
}

void Entity::addComponent(std::string componentName)
{
    std::string newName = componentName;
    if (this->componentList[newName] == NULL) {
        this->componentList[newName] = this->componentList.size();
        Component* newComponent = new Component(componentName);
        this->components.push_back(newComponent);
    }
    else {
        std::cout << "Error: Entity [" << this->name << "] already contain component [" << newName << "]" << std::endl;
    }
}

void Entity::addComponent(Component* component)
{
    std::string newName = component->componentName;
    if (this->componentList[newName] == NULL) {
        this->componentList[newName] = this->componentList.size();
        this->components.push_back(component);
    }
    else {
        std::cout << "Error: Entity [" << this->name << "] already contain component [" << newName << "]" << std::endl;
    }
}

void Entity::deleteComponent(int index)
{
    //this->components.erase(this->components.begin() + index);
}

Component* Entity::getComponentByIndex(int index)
{
    if (index < 0 || index >= this->components.size()) {
        std::cout << "Error: the index of getComponentByIndex(int index) out of bounds"<<std::endl;
        return nullptr;
    }
    return this->components[index];
}

std::string Entity::getName()
{
    return this->name;
}

void Entity::setName(std::string name)
{
    this->name = name;
}

Component* Entity::getComponentByName(std::string componentName)
{
    if (this->componentList[componentName] == NULL) {
        std::cout << "Error: Entity [" << this->name << "] do not contain component [" << componentName << "]" << std::endl;
        return nullptr;
    }
    return this->components[this->componentList[componentName]];
}

void Entity::render()
{
    for (Component* component : components) {
        component->renderGUI();
    }
}

void Entity::addTransformComponent()
{
    Component* Tcomponent = new transform_component();
    this->components.push_back(Tcomponent);
}
