#include "Fyredon/Other/pch.h"
#include "Fyredon/ECS/Entity.h"
#include "Fyredon/ECS/transform_component.h"
#include <iostream>
Entity::Entity()
{
    this->name = "new entity";
}

Entity::Entity(std::string name)
{
    this->name = name;
}

Entity::~Entity()
{
    for (Entity* entity : this->childs) {
        entity->~Entity();
        if (entity)free(entity);
    }
    for (Component* component: this->components) {
        component->~Component();
        if (component)free(component);
    }
}

void Entity::addComponent()
{
    std::string newName = "new component " + std::to_string((this->components.size() + 1));
    if (this->componentList.count(newName) == 0) {
        this->componentList[newName] = this->components.size();
        Component* newComponent = new Component(newName);
        newComponent->parent = this;
        this->components.push_back(newComponent);
    }
    else {
        std::cout << "Error: Entity [" << this->name << "] already contain component [" << newName << "]" << std::endl;
    } 
}

void Entity::addEntity(Entity* entity)
{
    entity->parent = this;
    this->childs.push_back(entity);
}

void Entity::addComponent(std::string componentName)
{
    std::string newName = componentName;
    if (this->componentList.count(newName) == 0) {
        this->componentList[newName] = this->components.size();
        Component* newComponent = new Component(componentName);
        newComponent->parent = this;
        this->components.push_back(newComponent);
    }
    else {
        std::cout << "Error: Entity [" << this->name << "] already contain component [" << newName << "]" << std::endl;
    }
}

void Entity::addComponent(Component* component)
{
    std::string newName = component->componentName;
    if (this->componentList.count(newName) == 0) {
        this->componentList[newName] = this->components.size();
        component->parent = this;
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
    if (this->componentList.count(componentName) == 0) {
        //std::cout << "Error: Entity [" << this->name << "] do not contain component [" << componentName << "]" << std::endl;
        return nullptr;
    }
    return this->components[this->componentList[componentName]];
}

void Entity::render()
{
    for (Component* component : components) {
        component->render();
    }
}

void Entity::tick()
{
    for (Component* component : components) {
        component->tick();
    }
}

void Entity::play()
{
    for (Component* component : components) {
        component->play();
    }
}

void Entity::init()
{
    for (Component* component : components) {
        component->init();
    }
}

void Entity::stop()
{
    for (Component* component : components) {
        component->stop();
    }
}

void Entity::reset()
{
    for (Component* component : components) {
        component->reset();
    }
}

void Entity::addTransformComponent()
{
    Component* Tcomponent = new transform_component();
    if (this->componentList.count(Tcomponent->componentName) == 0) {
        this->componentList[Tcomponent->componentName] = this->components.size();
        Tcomponent->parent = this;
        this->components.push_back(Tcomponent);
    }
    else {
        std::cout << "Error: Entity [" << this->name << "] already contain component [Transform]" << std::endl;
    }
}
