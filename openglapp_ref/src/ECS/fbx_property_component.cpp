#include "ECS/fbx_property_component.h"

fbx_property_component::fbx_property_component()
{
	this->componentName = "Property";
}

fbx_property_component::fbx_property_component(float startTime, float endTime, float speed) {
	this->componentName = "Property";
	this->startTime = startTime;
	this->endTime = endTime;
	this->speed = speed;
}

void fbx_property_component::GUI_struct()
{
	ImGui::InputFloat("startTime", &this->startTime);
	ImGui::InputFloat("endTime", &this->endTime);
	ImGui::InputFloat("speed", &this->speed);
}
