#include "Fyredon/Other/pch.h"
#include "Fyredon/ECS/fbx_property_component.h"

fbx_property_component::fbx_property_component()
{
	this->componentName = "Animation";
	this->memmory_size = sizeof(fbx_property_component);
}

fbx_property_component::fbx_property_component(float startTime, float endTime, float speed) {
	this->componentName = "Animation";
	this->startTime = startTime;
	this->endTime = endTime;
	this->speed = speed;
	this->memmory_size = sizeof(fbx_property_component);
}

void fbx_property_component::GUI_struct()
{
	ImGui::InputFloat("speed", &this->speed);
	ImGui::InputFloat("start time", &this->startTime);
	ImGui::InputFloat("end time", &this->endTime);
	ImGui::InputFloat("current frame", &this->currentFrameIndex);
}
