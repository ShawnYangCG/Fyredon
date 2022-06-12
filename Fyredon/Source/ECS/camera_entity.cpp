#include "Fyredon/Other/pch.h"
#include "Fyredon/ECS/camera_entity.h"

camera_entity::camera_entity()
{
	this->setName("Camera");
	this->addTransformComponent();
	this->addComponent(new camera_component(new Fyredon::Camera()));
}

camera_entity::camera_entity(Fyredon::Camera* camera)
{
	this->setName("Camera");
	this->addTransformComponent();
	this->addComponent(new camera_component(camera));
}
