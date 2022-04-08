#pragma once
#include "ECS/ECS.h"
// 1.
// include your own header file
#include "camera.h"
class Camera;
// 2.
// inherit the entity class
class camera_entity : public Entity
{
public:
	// 3.
	// add your own veriable here

	// 4.
	// rewrite the constructor
	camera_entity(std::string name, glm::vec3 pos, float target_offset, float fov, float ar, bool freelook, float width, float height);
};
// 5.
// don't forget to add this header in ECS.h
