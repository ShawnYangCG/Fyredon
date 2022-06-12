#include "Fyredon/Other/pch.h"
#include "Fyredon/ECS/navigation_entity.h"

navigation_entity::navigation_entity(){
	this->setName("Navigation System");
	this->navigationSys = new NavigationSystem();
	this->addComponent();
}

navigation_entity::navigation_entity(NavigationSystem* navigationSys)
{
	this->setName("Navigation System");
	this->navigationSys = navigationSys;
	this->addComponent();
}

void navigation_entity::play()
{
	navigationSys->tick();
}

void navigation_entity::setMap(Entity* entity)
{
	this->navigationSys->map = entity;
}

void navigation_entity::setDest(Entity* entity)
{
	this->navigationSys->destPoint = entity;
}

void navigation_entity::addMoveObject(Entity* entity)
{
	this->navigationSys->addEntity(entity);
}
