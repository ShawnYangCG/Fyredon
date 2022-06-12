#include "Fyredon/Other/pch.h"
#include "Fyredon/ECS/test_move_component.h"

test_move_component::test_move_component() {
	this->componentName = "Move test";
	this->memmory_size = sizeof(test_move_component);
}

void test_move_component::GUI_struct()
{
	ImGui::InputFloat("Speed", &this->speed);
}

glm::vec3 test_move_component::getCurrentPosition()
{
	Entity* currentObject = this->parent;
	transform_component* transCom = (transform_component*)currentObject->getComponentByName("Transform");
	return glm::vec3(transCom->matrixTranslation[0], transCom->matrixTranslation[1], transCom->matrixTranslation[2]);
}

void test_move_component::setPosition(glm::vec3 pos)
{
	Entity* currentObject = this->parent;
	transform_component* transCom = (transform_component*)currentObject->getComponentByName("Transform");
	transCom->matrixTranslation[0] = pos[0];
	transCom->matrixTranslation[1] = pos[1];
	transCom->matrixTranslation[2] = pos[2];
}

void test_move_component::setRotation(float angle)
{
	Entity* currentObject = this->parent;
	transform_component* transCom = (transform_component*)currentObject->getComponentByName("Transform");
	transCom->matrixRotation[1] = angle;
}
bool test_move_component::moveOneStep(glm::vec3 dest, float deltaTime) 
{
	glm::vec3 src = getCurrentPosition();
	glm::vec3 forwardVector = glm::normalize(dest - src);
	if (glm::length(forwardVector) > 0) {
		glm::vec3 rotateAxis = glm::normalize(glm::cross(forwardVector, this->front));
		if (glm::length(rotateAxis) != 0) {

			float cosAngle = 1;
			if (forwardVector[0] < 0)
				cosAngle = -1;
			cosAngle *= acos(glm::dot(this->front, forwardVector)) * (180 / 3.1415926);
			setRotation(cosAngle);
		}
		glm::vec3 after = src + forwardVector * this->speed * deltaTime;
		if (glm::dot((dest - after), forwardVector) <= 0) {
			after = dest;
			setPosition(after);
			return true;
		}
		setPosition(after);
		return false;
	}
	return true;
}

