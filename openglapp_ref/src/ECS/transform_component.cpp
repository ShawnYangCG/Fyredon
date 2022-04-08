#include "ECS/transform_component.h"

transform_component::transform_component() {
	this->componentName = "Transform";
	this->memmory_size = sizeof(transform_component);
}

void transform_component::GUI_struct()
{
	ImGui::InputFloat3("Position", this->matrixTranslation);
	ImGui::InputFloat3("Rotation", this->matrixRotation);
	ImGui::InputFloat3("Scale", this->matrixScale);
}
