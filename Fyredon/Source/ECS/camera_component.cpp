#include "Fyredon/Other/pch.h"
#include "Fyredon/ECS/camera_component.h"

camera_component::camera_component(Fyredon::Camera* camera)
{
	this->componentName = "Camera";
	this->camera = camera;
	this->memmory_size = sizeof(camera_component);
}

void camera_component::GUI_struct()
{

}
