#pragma once
#include "Fyredon/ECS/World.h"
#include "Fyredon/ECS/Entity.h"
#include "Fyredon/ECS/mesh_component.h"
#include "Fyredon/ECS/camera_component.h"
#include "Fyredon/ECS/transform_component.h"
#include "Fyredon/Render/Camera.h"
#include "Fyredon/Render/Renderer.h"
class World;
class EngineRenderSystem
{
public:
	World* world;
	Fyredon::Camera* camera;
	Fyredon::Renderer* renderer;
	glm::vec3* light_pos;
	float* light_strength;

	EngineRenderSystem(World* world, Fyredon::Renderer* renderer);
	~EngineRenderSystem();
	void tick();
	void setCamera(Fyredon::Camera* camera);
	void setLight(glm::vec3* light_pos, float* light_strength);
	void renderMesh(Fyredon::Mesh* mesh);
};

