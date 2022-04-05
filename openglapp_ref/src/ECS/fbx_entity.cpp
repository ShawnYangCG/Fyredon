#include "ECS/fbx_entity.h"

fbx_entity::fbx_entity(std::string name, std::string filePath, float startTime, float endTime, float timeStep)
{
	this->setName(name);
	this->mesh = new fbx_mesh(name.data(), filePath.data());
	this->mesh->load_obj(false);
	this->addTransformComponent();
	Component* property = new fbx_property_component(startTime, endTime, timeStep);
	this->addComponent(property);
}
