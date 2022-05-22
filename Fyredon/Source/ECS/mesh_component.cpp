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
	bounds.min = glm::vec3(0.f), bounds.max = glm::vec3(1.f); 
	parms.file_path = "..//";
	ren_idx = 0; 

	World& world = World::get_world();

	componentName = std::string("Mesh_") + std::to_string(world.counts.mesh_count++);

	// Default Mesh OBJ 
	mesh = new Fyredon::Mesh_OBJ(componentName.c_str(), "..//Assets//Mesh//pighead.obj");
	mesh->load_obj();
	mesh->scale(glm::vec3(2.f));
	mesh->translate(glm::vec3(0.f, 0.f, 1.f));

	ren_idx = Fyredon::Renderer::get_renderer().submit_mesh(this->mesh);

	this->parent = parent;
	this->memmory_size = sizeof(mesh_component);
}

// Remove mesh from render_list if component deleted, then delete inner mesh.
mesh_component::mesh_component::~mesh_component()
{
	Fyredon::Renderer::get_renderer().delete_mesh(ren_idx);
	delete mesh; 
}

void mesh_component::mesh_changed()
{
	/*
	std::ifstream test(parms.file_path);
	if (!test.is_open()) {
		std::cerr << "ERROR::Mesh_Component::Invalid Mesh File Path." << std::endl;
		return;
	} */

	if (mesh) {
		Fyredon::Renderer::get_renderer().delete_mesh(ren_idx);
		delete mesh;
	}
	mesh = new Fyredon::Mesh_OBJ(componentName.c_str(), parms.file_path);
	mesh->load_obj();
	ren_idx = Fyredon::Renderer::get_renderer().submit_mesh(this->mesh);
}

void mesh_component::GUI_struct()
{
	static char file_buf[512]{ "..//Assets//Mesh//pighead.obj" };
	static char diff_buf[512]{ "..//" };
	static char spec_buf[512]{ "..//" };
	
	std::string vertex = "Vertex: " + std::to_string(this->mesh->vert_count);
	std::string face   = "Face: " + std::to_string(this->mesh->index_count / 3);
	ImGui::Text(vertex.data());
	ImGui::Text(face.data());

	if (ImGui::InputText("Mesh File", file_buf, 512)) parms.file_path = file_buf; 
	if (ImGui::Button("Load Mesh")) {
		parms.file_path = file_buf; 
		mesh_changed();
	}

	// Draw Material GUI
}

// Info : Update Camera State per tick (No GUI Tick here, this is done via Component::tick_gui() in editortick)
void mesh_component::tick()
{
	//
}