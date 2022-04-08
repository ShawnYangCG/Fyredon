#include "ECS/World.h"

World::World()
{

}

void World::addEntity(Entity* entity)
{
    if (this->entityList[entity->getName()] == NULL) {
        this->entityList[entity->getName()] = this->entityList.size();
        this->entities.push_back(entity);
    }
    else {
        std::cout << "Error: World already contain entity [" << entity->getName() << "]" << std::endl;
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
    if (this->entityList[EntityName] == NULL) {
        std::cout << "Error: World do not contain entity [" << EntityName << "]" << std::endl;
        return nullptr;
    }
    return this->entities[this->entityList[EntityName]];
}
