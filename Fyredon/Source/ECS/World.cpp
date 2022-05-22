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
#include "Fyredon/ECS/shaderball_entity.h"

// ================= Singleton Instance =================
World::World() : gui_org(nullptr) 
{
    counts.terrain_entity_count = 0, counts.mesh_count = 0, counts.camera_count = 0;
    counts.camera_entity_count = 0, counts.light_count = 0, counts.transform_count = 0; 
}

World World::world; 

World& World::get_world() { return world; }

// Info : Entity ownership within World, each entitiy is responsible for deleting its components. 
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

// Info : Shawn I think we should remove this as ownership of entitiy is now within world initally ? - Niall.
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
    Fyredon::terrain_entity* entity = new Fyredon::terrain_entity;
    this->addEntity(entity);
}

void World::create_shaderball_entity()
{
    Fyredon::shaderball_entity* entity = new Fyredon::shaderball_entity();
    this->addEntity(entity);
}

void World::create_camera_entity()
{
    Fyredon::camera_entity* entity = new Fyredon::camera_entity();
    this->addEntity(entity);
}

// Info : overload checks for named camera (Scene or Game) to override pos and reference explicitlly within world. 
void World::create_camera_entity(const std::string& name)
{
    Fyredon::camera_entity* entity = new Fyredon::camera_entity();

    entity->setName(name);
    entity->getComponentByIndex(0)->componentName = name; 

    // Hacky way to set default init for each camera type
    auto& renderer = Fyredon::Renderer::get_renderer();
    if (name == "Scene_Camera") 
    {
        scene_cam_entity = entity; 

        Fyredon::camera_component *cam_comp = static_cast<Fyredon::camera_component*>(entity->getComponentByIndex(0));
        glm::vec3 scam_pos(3.f, 1.f, -2.f);
        cam_comp->set_camera_pos(scam_pos);
    }
    if (name == "Game_Camera")  
    {
        game_cam_entity = entity;

        // Set as default active camera for renderer
        Fyredon::Camera* raw_cam = static_cast<Fyredon::camera_component*>(entity->getComponentByIndex(0))->camera;
        renderer.set_active_camera(raw_cam);
    }

    // Add to ECS World Entity List
    this->addEntity(entity);
    // Probs should use camera_array oppose to just general entities, but we also store in cam ptrs. 
    cameraList[name] = entityList.size() - 1; 
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

// Info : Tick via application Delta Time
void World::tick(float Dt)
{
    for (Entity* entity : entities)
    {
        entity->tick();
    }

    time.total_editor_time += Dt; 
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
