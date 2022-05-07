// Copyright Fyredon. All Rights Reserved.
/*=============================================================================
    World.cpp: ECS Game World : Shawn, Niall
=============================================================================*/
#include "Fyredon/Other/pch.h"
// Implements
#include "Fyredon/ECS/World.h"

// ================= Singleton Instance =================
World::World() : gui_org(nullptr) {}

World World::world; 

World& World::get_world() { return world; }

// Assume ownership is in world class
World::~World()
{
    if (entities.size()) {
        for (Entity* entity : entities)
        {
            if (entity) delete entity; 
        }
    }

}

// ================= ECS World Class Implementation =================

World::World(GUI_PanelOrgnizer* gui_org)
{
    this->gui_org = gui_org;
}

void World::addEntity(Entity* entity)
{
    if (this->entityList.count(entity->getName()) == 0) {
        this->entityList[entity->getName()] = this->entities.size();
        this->entities.push_back(entity);
    }
    else {
        //std::cout << "Error: World already contain entity [" << entity->getName() << "]" << std::endl;
        entity->setName(entity->getName() +" "+ std::to_string(this->entities.size()));
        this->entityList[entity->getName()] = this->entities.size();
        this->entities.push_back(entity);
    }
}

void World::addCamera(Entity* entity)
{
    if (this->cameraList.count(entity->getName()) == 0) {
        this->cameraList[entity->getName()] = this->cameras.size();
        this->cameras.push_back(entity);
    }
    else {
        //std::cout << "Error: World already contain entity [" << entity->getName() << "]" << std::endl;
        entity->setName(entity->getName() + " " + std::to_string(this->cameras.size()));
        this->cameraList[entity->getName()] = this->cameras.size();
        this->cameras.push_back(entity);
    }
}

void World::init()
{
    for (int i = 0; i < this->entities.size(); i++) {
        this->entities[i]->init();
    }
}

void World::tick()
{
    for (int i = 0; i < this->entities.size(); i++) {
        this->entities[i]->tick();
    }
}

void World::reset()
{
    for (int i = 0; i < this->entities.size(); i++) {
        this->entities[i]->reset();
    }
}

void World::stop()
{
    for (int i = 0; i < this->entities.size(); i++) {
        this->entities[i]->stop();
    }
}

Entity* World::getEntityByIndex(int index)
{
    if (index < 0 || index >= this->entities.size()) {
        std::cout << "Error: the index of getEntityByIndex(int index) out of bounds" << std::endl;
        return nullptr;
    }
    return this->entities[index];
}

Entity* World::getEntityByName(std::string EntityName)
{
    if (this->entityList.count(EntityName) == 0) {
        std::cout << "Error: World do not contain entity [" << EntityName << "]" << std::endl;
        return nullptr;
    }
    return this->entities[this->entityList[EntityName]];
}

Entity* World::getCameraByIndex(int index)
{
    if (index < 0 || index >= this->cameras.size()) {
        std::cout << "Error: the index of getEntityByIndex(int index) out of bounds" << std::endl;
        return nullptr;
    }
    return this->cameras[index];
}

Entity* World::getCameraByName(std::string CameraName)
{
    if (this->cameraList.count(CameraName) == 0) {
        std::cout << "Error: World do not contain entity [" << CameraName << "]" << std::endl;
        return nullptr;
    }
    return this->cameras[this->cameraList[CameraName]];
}
