// Copyright Fyredon. All Rights Reserved.
/*=============================================================================
	Mesh_Component.cpp: ECS Mesh Component : Niall, Shawn
=============================================================================*/
#include "Fyredon/Other/pch.h"
// Implements
#include "Fyredon/ECS/mesh_component.h"

#include "Fyredon/ECS/World.h"

mesh_component::mesh_component(Entity* parent)
{
	World& world = World::get_world();

	componentName = std::string("Mesh_") + std::to_string(world.counts.mesh_count++);

	// Default Mesh OBJ 
	mesh = new Fyredon::Mesh_OBJ(componentName.c_str(), "..//Assets//Mesh//pighead.obj");
	mesh->load_obj();
	mesh->scale(glm::vec3(2.f));
	mesh->translate(glm::vec3(0.f, 0.2f, 2.f));

	Fyredon::Renderer::get_renderer().submit_mesh(this->mesh);

	this->memmory_size = sizeof(mesh_component);
}

// Remove mesh from render_list if component deleted, then delete mesh.
mesh_component::mesh_component::~mesh_component()
{
	//auto& ren = Fyredon::Renderer::get_renderer();
	//ren.delete_mesh ...;

	delete mesh; 
}

void mesh_component::GUI_struct()
{
	std::string vertex = "Vertex: " + std::to_string(this->mesh->vert_count);
	std::string face = "Face: " + std::to_string(this->mesh->index_count / 3);
	ImGui::Text(vertex.data());
	ImGui::Text(face.data());

	// Draw Material GUI
}

void mesh_component::tick()
{
	this->renderGUI();
}