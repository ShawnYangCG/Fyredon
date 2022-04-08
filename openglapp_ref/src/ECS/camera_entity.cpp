#include "ECS/camera_entity.h"

camera_entity::camera_entity(std::string name, glm::vec3 pos, float target_offset, float fov, float ar, bool freelook, float width, float height)
{
	this->setName(name);
	this->addTransformComponent();
	Camera* camera = new Camera(glm::vec3(3.f, 1.f, -1.f), 1.f, 80.f, width / height, true);

}
