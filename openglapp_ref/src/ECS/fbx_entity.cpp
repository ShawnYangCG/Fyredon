#include "ECS/fbx_entity.h"

fbx_entity::fbx_entity(std::string name, std::string filePath, float startTime, float endTime, float timeStep)
{
	this->setName(name);
	fbx_mesh* FBXmesh;
	FBXmesh = new fbx_mesh(name.data(), filePath.data());
	FBXmesh->load_obj(false);
	this->addTransformComponent();
	Component* MESHcomponent = new mesh_component(FBXmesh);
	this->addComponent(MESHcomponent);
	Component* property = new fbx_property_component(startTime, endTime, timeStep);
	this->addComponent(property);
}
