#include "Fyredon/Other/pch.h"
#include "Fyredon/ECS/System/EngineRenderSystem.h"

EngineRenderSystem::EngineRenderSystem(World* world, Fyredon::Renderer* renderer)
{
	this->world = world;
	this->renderer = renderer;
}

EngineRenderSystem::~EngineRenderSystem()
{
	if (renderer) {
		renderer->~Renderer();
		free(renderer);
	}
	if (camera) {
		camera->~Camera();
		free(camera);
	}
}

void EngineRenderSystem::tick()
{
	// render camera
	for (Entity* entity : this->world->cameras) {
		camera_component* caCom = (camera_component*)entity->getComponentByName("Camera");
		transform_component* transf = (transform_component*)entity->getComponentByName("Transform");
		if (transf->isChanged) {
			Fyredon::Mesh* mesh = new Fyredon::Mesh(*(caCom->outlook));

			float TranslationX, TranslationY, TranslationZ;
			float ScaleX, ScaleY, ScaleZ;
			float RotationX, RotationY, RotationZ;

			TranslationX = transf->matrixTranslation[0];
			TranslationY = transf->matrixTranslation[1];
			TranslationZ = transf->matrixTranslation[2];
			mesh->translate(glm::vec3(TranslationX, TranslationY, TranslationZ));

			RotationX = transf->matrixRotation[0];
			RotationY = transf->matrixRotation[1];
			RotationZ = transf->matrixRotation[2];
			mesh->local_rotate(RotationX, RotationY, RotationZ);

			ScaleX = transf->matrixScale[0];
			ScaleY = transf->matrixScale[1];
			ScaleZ = transf->matrixScale[2];
			mesh->scale(glm::vec3(ScaleX, ScaleY, ScaleZ));

			delete caCom->outlook_render;
			caCom->outlook_render = new Fyredon::Mesh(*mesh); 
			delete mesh;
		}
	}
	// render object
	for (Entity* entity : this->world->entities) {
		if (entity->getComponentByName("Mesh")) {
			mesh_component* meCom = (mesh_component*)entity->getComponentByName("Mesh");
			transform_component* transf = (transform_component*)entity->getComponentByName("Transform");
			if (transf->isChanged) {
				Fyredon::Mesh* mesh = (Fyredon::Mesh*)malloc(sizeof(Fyredon::Mesh));
				memcpy((void*)mesh, (void*)meCom->mesh, sizeof(Fyredon::Mesh));
				float TranslationX, TranslationY, TranslationZ;
				float ScaleX, ScaleY, ScaleZ;
				float RotationX, RotationY, RotationZ;

				TranslationX = transf->matrixTranslation[0];
				TranslationY = transf->matrixTranslation[1];
				TranslationZ = transf->matrixTranslation[2];
				mesh->translate(glm::vec3(TranslationX, TranslationY, TranslationZ));

				RotationX = transf->matrixRotation[0];
				RotationY = transf->matrixRotation[1];
				RotationZ = transf->matrixRotation[2];
				mesh->local_rotate(RotationX, RotationY, RotationZ);

				ScaleX = transf->matrixScale[0];
				ScaleY = transf->matrixScale[1];
				ScaleZ = transf->matrixScale[2];
				mesh->scale(glm::vec3(ScaleX, ScaleY, ScaleZ));
				memcpy((void*)meCom->mesh_render, (void*)mesh, sizeof(Fyredon::Mesh));
				free(mesh);
			}
		}
	}
}

void EngineRenderSystem::setCamera(Fyredon::Camera* camera)
{
	this->camera = camera;
}

void EngineRenderSystem::setLight(glm::vec3* light_pos, float* light_strength)
{
	this->light_pos = light_pos;
	this->light_strength = light_strength;
}

void EngineRenderSystem::renderMesh(Fyredon::Mesh* mesh)
{
	mesh->shader.setVec("camPos_world", camera->position);
	mesh->shader.setVec("lightPos_world", *light_pos);
	mesh->shader.setFloat("lightStr", *light_strength);
	mesh->set_cameraTransform(camera->get_view_matrix(), camera->get_proj_matrix());
	mesh->render();
}
