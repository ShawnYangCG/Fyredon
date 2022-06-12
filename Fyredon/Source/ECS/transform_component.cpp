#include "Fyredon/Other/pch.h"
#include "Fyredon/ECS/transform_component.h"

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

void transform_component::tick()
{
	this->isChanged = false;
	checkChanged();
}

void transform_component::reset()
{
	Component::reset();
	std::cout << this->matrixTranslation[0] 
		<< this->matrixTranslation[1]
		<< this->matrixTranslation[2] 
		<< std::endl;
	this->isChanged = true;
}

void transform_component::checkChanged()
{
	if ((matrixTranslation_check[0] != matrixTranslation[0] ||
		matrixTranslation_check[1] != matrixTranslation[1] ||
		matrixTranslation_check[2] != matrixTranslation[2] ||
		matrixRotation_check[0] != matrixRotation[0] ||
		matrixRotation_check[1] != matrixRotation[1] ||
		matrixRotation_check[2] != matrixRotation[2] ||
		matrixScale_check[0] != matrixScale[0] ||
		matrixScale_check[1] != matrixScale[1] ||
		matrixScale_check[2] != matrixScale[2]) 
		&& this->isChanged == false
		) {
		matrixTranslation_check[0] = matrixTranslation[0];
		matrixTranslation_check[1] = matrixTranslation[1];
		matrixTranslation_check[2] = matrixTranslation[2];
		matrixRotation_check[0] = matrixRotation[0];
		matrixRotation_check[1] = matrixRotation[1];
		matrixRotation_check[2] = matrixRotation[2];
		matrixScale_check[0] = matrixScale[0];
		matrixScale_check[1] = matrixScale[1];
		matrixScale_check[2] = matrixScale[2];
		this->isChanged = true;
	}
}
