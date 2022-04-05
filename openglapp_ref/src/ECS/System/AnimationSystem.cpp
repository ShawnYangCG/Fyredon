#include "ECS/System/AnimationSystem.h"

AnimationSystem::AnimationSystem()
{
}

void AnimationSystem::addFbxEntity(fbx_entity* entity)
{
	if (this->entityList[entity->getName()] == NULL) {
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
	if (this->entityList[EntityName] == NULL) {
		std::cout << "Error: World do not contain entity [" << EntityName << "]" << std::endl;
		return nullptr;
	}
	return this->entities[this->entityList[EntityName]];
}

void AnimationSystem::run()
{
	for (fbx_entity* entity : this->entities) {
		if (entity->currentFrameIndex < entity->mesh->fbx_mesh_all_fream.size()) {
			entity->mesh->playCurrentFrame((int)entity->currentFrameIndex);
			float speed = ((fbx_property_component*)entity->getComponentByName("Property"))->speed;
			entity->currentFrameIndex += speed;
		}
		else {
			entity->currentFrameIndex = (int)entity->currentFrameIndex % entity->mesh->fbx_mesh_all_fream.size();
			entity->mesh->playCurrentFrame((int)entity->currentFrameIndex);
		}
	}
}
