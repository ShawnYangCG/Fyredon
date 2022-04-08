#pragma once
#include "ECS/World.h"
#include "ECS/Entity.h"
#include "ECS/mesh_component.h"
#include "ECS/transform_component.h"
#include "fbx_mesh.h"
#include "camera.h"
class World;
class Camera;
class EngineRenderSystem
{
public:
	World* world;
	Camera* camera;
	glm::vec3* light_pos;
	float* light_strength;

	EngineRenderSystem(World* world);
	void tick();
	void setCamera(Camera* camera);
	void setLight(glm::vec3* light_pos, float* light_strength);
};

