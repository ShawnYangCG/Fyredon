#include "Fyredon/Other/pch.h"
#include "Fyredon/ECS/System/EngineRenderSystem.h"

EngineRenderSystem::EngineRenderSystem(World* world, Fyredon::Renderer* renderer)
{
	this->world = world;
	this->renderer = renderer;
}

void EngineRenderSystem::tick()
{
	for (Entity* entity : this->world->entities) {
		if (entity->getComponentByName("Mesh")) {
			mesh_component* meCom = (mesh_component*)entity->getComponentByName("Mesh");
			transform_component* transf = (transform_component*)entity->getComponentByName("Transform");
			if (transf->isChanged) {
				free(meCom->mesh_render);
				Fyredon::Mesh* mesh = (Fyredon::Mesh*)malloc(sizeof(Fyredon::Mesh));
				memcpy((void*)mesh, (void*)meCom->mesh, sizeof(Fyredon::Mesh));
				meCom->mesh_render = mesh;
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
				renderer->submit_primitive(meCom->mesh_render);
			}
			
			/*
			meCom->mesh_render->shader.setVec("camPos_world", camera->position);
			meCom->mesh_render->shader.setVec("lightPos_world", *light_pos);
			meCom->mesh_render->shader.setFloat("lightStr", *light_strength);
			meCom->mesh_render->set_cameraTransform(camera->get_view_matrix(), camera->get_proj_matrix());

			meCom->mesh_render->render();
			*/
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
