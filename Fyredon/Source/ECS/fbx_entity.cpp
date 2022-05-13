#include "Fyredon/Other/pch.h"
#include "Fyredon/ECS/fbx_entity.h"

fbx_entity::fbx_entity(std::string name, std::string filePath, float startTime, float endTime, float timeStep)
{
	this->setName(name);
	FBXmesh = new fbx_mesh(name.data(), filePath.data());
	FBXmesh->load_obj(false);
	
	
}
