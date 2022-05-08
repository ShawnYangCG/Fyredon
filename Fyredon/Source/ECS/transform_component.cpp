#include "Fyredon/Other/pch.h"
// Implements
#include "Fyredon/ECS/transform_component.h"

#include "Fyredon/ECS/World.h"

transform_component::transform_component() 
{
	this->componentName = std::string("Transform_") + std::to_string(World::get_world().counts.transform_count++);
	//this->memmory_size = sizeof(transform_component);
}

void transform_component::GUI_struct()
{
	isChanged = false;
	if (ImGui::InputFloat3("Position", matrix_translation))
	{
		isChanged = true;
	}
	if (ImGui::InputFloat3("Rotation", matrix_rotation))
	{
		isChanged = true;
	}
	if (ImGui::InputFloat3("Scale", matrix_scale))
	{
		isChanged = true;
	}
}

void transform_component::tick()
{
	// Pass transform component to parent components of entity.
	if (isChanged)
	{
		isChanged = false; 

	}

	// Tick Parent
	Component::tick();
}