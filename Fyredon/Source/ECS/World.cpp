// Copyright Fyredon. All Rights Reserved.
/*=============================================================================
    World.cpp: ECS Game World : Shawn, Niall
=============================================================================*/
#include "Fyredon/Other/pch.h"
// Implements
#include "Fyredon/ECS/World.h"

// Entites
#include "Fyredon/ECS/terrain_entity.h"
#include "Fyredon/ECS/camera_entity.h"

// ================= Singleton Instance =================
World::World() : gui_org(nullptr) 
{
    counts.terrain_entity_count = 0, counts.mesh_count = 0, counts.camera_count = 0;
    counts.camera_entity_count = 0, counts.light_count = 0; 
}

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

// ================= Entity Creation =================

void World::create_terrain_entity()
{
    terrain_entity* entity = new terrain_entity;
    this->addEntity(entity);
    counts.terrain_entity_count++;
}

void World::create_camera_entity()
{
    Fyredon::camera_entity* entity = new Fyredon::camera_entity();
    this->addEntity(entity);
    counts.camera_entity_count++;
}

void World::create_doll_entity()
{

}

void World::create_empty_entity()
{

}

// ================= Core World Functions =================

void World::init()
{
    for (Entity* entity : entities)
    {
        entity->init();
    }
}

void World::tick()
{
    for (Entity* entity : entities)
    {
        entity->tick();
    }
}

void World::reset()
{
    for (Entity* entity : entities)
    {
        entity->reset();
    }
}

void World::stop()
{
    for (Entity* entity : entities)
    {
        entity->stop();
    }
}

// ================= Getters =================
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
