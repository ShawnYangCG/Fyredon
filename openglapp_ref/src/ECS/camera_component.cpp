#include "ECS/camera_component.h"

camera_component::camera_component(Camera* camera)
{
	this->componentName = "Camera";
	this->camera = camera;
	this->memmory_size = sizeof(camera_component);
}

void camera_component::GUI_struct()
{

}
