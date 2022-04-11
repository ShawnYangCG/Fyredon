#include "ECS/System/AnimationSystem.h"

AnimationSystem::AnimationSystem()
{
}

void AnimationSystem::addFbxEntity(fbx_entity* entity)
{
	if (this->entityList.count(entity->getName()) == 0) {
		this->entityList[entity->getName()] = this->entityList.size();
		this->entities.push_back(entity);
	}
	else {
		std::cout << "Error: Animation already contain entity [" << entity->getName() << "]" << std::endl;
	}
}

fbx_entity* AnimationSystem::getEntityByIndex(int index)
{
	if (index < 0 || index >= this->entities.size()) {
		std::cout << "Error: the index of getEntityByIndex(int index) out of bounds" << std::endl;
		return nullptr;
	}
	return this->entities[index];
}

fbx_entity* AnimationSystem::getEntityByName(std::string EntityName)
{
	if (this->entityList.count(EntityName) == 0) {
		std::cout << "Error: World do not contain entity [" << EntityName << "]" << std::endl;
		return nullptr;
	}
	return this->entities[this->entityList[EntityName]];
}

void AnimationSystem::run()
{
	for (fbx_entity* entity : this->entities) {
		Mesh* mesh = ((mesh_component*)(entity->getComponentByName("Mesh")))->mesh;
		float *currentFrame = &((fbx_property_component*)entity->getComponentByName("Animation"))->currentFrameIndex;
		if (*currentFrame < ((fbx_mesh*)mesh)->fbx_mesh_all_fream.size()) {
			((fbx_mesh*)mesh)->playCurrentFrame((int)*currentFrame);
			float speed = ((fbx_property_component*)entity->getComponentByName("Animation"))->speed;
			*currentFrame += speed;

		}
		else {
			*currentFrame = (int)*currentFrame % ((fbx_mesh*)mesh)->fbx_mesh_all_fream.size();
			((fbx_mesh*)mesh)->playCurrentFrame((int)*currentFrame);
		}
	}
}
