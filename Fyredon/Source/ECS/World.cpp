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
    counts.camera_entity_count = 0, counts.light_count = 0, counts.transform_count = 0; 
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

// Info : Checks for named camera (Scene or Game) to also reference explicitlly. 
void World::create_camera_entity(const std::string& name)
{
    Fyredon::camera_entity* entity = new Fyredon::camera_entity();

    entity->setName(name);
    entity->getComponentByIndex(0)->componentName = name; 

    // Hacky way to set default init for each camera type
    auto& renderer = Fyredon::Renderer::get_renderer();
    if (name == "Scene_Camera") 
    {
        Fyredon::camera_component *cam_comp = static_cast<Fyredon::camera_component*>(entity->getComponentByIndex(0));
        Fyredon::Camera* raw_cam = cam_comp->camera;
        // Scene Camera default pos
        cam_comp->parms.pos[0] = 5.f, cam_comp->parms.pos[1] = 1.f, cam_comp->parms.pos[2] = -2.f;
        raw_cam->position = glm::vec3(cam_comp->parms.pos[0], cam_comp->parms.pos[1], cam_comp->parms.pos[2]); 

        scene_cam_entity = entity;
    }

    if (name == "Game_Camera")  
    {
        game_cam_entity = entity;

        // Set as default active camera for renderer
        Fyredon::Camera* raw_cam = static_cast<Fyredon::camera_component*>(entity->getComponentByIndex(0))->camera;
        renderer.set_active_camera(raw_cam);
    }

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
