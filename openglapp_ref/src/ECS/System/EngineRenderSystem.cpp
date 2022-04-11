#include "ECS/System/EngineRenderSystem.h"

EngineRenderSystem::EngineRenderSystem(World* world)
{
	this->world = world;
}

void EngineRenderSystem::tick()
{
	for (Entity* entity : this->world->entities) {
		if (entity->getComponentByName("Mesh")) {
			mesh_component* meCom = (mesh_component*)entity->getComponentByName("Mesh");
			Mesh* mesh = (Mesh*)malloc(sizeof(Mesh));
			memcpy((void*)mesh, (void*)meCom->mesh, sizeof(Mesh));
			float TranslationX, TranslationY, TranslationZ;
			float ScaleX, ScaleY, ScaleZ;
			float RotationX, RotationY, RotationZ;
			transform_component* transf = (transform_component*)entity->getComponentByName("Transform");
			TranslationX = transf->matrixTranslation[0];
			TranslationY = transf->matrixTranslation[1];
			TranslationZ = transf->matrixTranslation[2];
			mesh->translate(glm::vec3(TranslationX, TranslationY, TranslationZ));

			RotationX = transf->matrixRotation[0];
			RotationY = transf->matrixRotation[1];
			RotationZ = transf->matrixRotation[2];
			mesh->localRotate(RotationX, RotationY, RotationZ);

			ScaleX = transf->matrixScale[0];
			ScaleY = transf->matrixScale[1];
			ScaleZ = transf->matrixScale[2];
			mesh->scale(glm::vec3(ScaleX, ScaleY, ScaleZ));

			mesh->shader.setVec("camPos_world", camera->Cam_Pos);
			mesh->shader.setVec("lightPos_world", *light_pos);
			mesh->shader.setFloat("lightStr", *light_strength);
			mesh->set_cameraTransform(camera->get_ViewMatrix(), camera->get_PerspMatrix());
			mesh->render();
			free(mesh);
		}
	}
}

void EngineRenderSystem::setCamera(Camera* camera)
{
	this->camera = camera;
}

void EngineRenderSystem::setLight(glm::vec3* light_pos, float* light_strength)
{
	this->light_pos = light_pos;
	this->light_strength = light_strength;
}

void EngineRenderSystem::renderMesh(Mesh* mesh)
{
	mesh->shader.setVec("camPos_world", camera->Cam_Pos);
	mesh->shader.setVec("lightPos_world", *light_pos);
	mesh->shader.setFloat("lightStr", *light_strength);
	mesh->set_cameraTransform(camera->get_ViewMatrix(), camera->get_PerspMatrix());
	mesh->render();
}
