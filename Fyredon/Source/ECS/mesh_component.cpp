#include "Fyredon/Other/pch.h"
#include "Fyredon/ECS/mesh_component.h"

mesh_component::mesh_component(Fyredon::Mesh* mesh)
{
	this->componentName = "Mesh";
	this->mesh = mesh;
	this->memmory_size = sizeof(mesh_component);
}

void mesh_component::GUI_struct()
{
	std::string vector = "Vector: " + std::to_string(this->mesh->vert_data.size());
	std::string edge = "Edge:";
	std::string face = "Face:";
	ImGui::Text(vector.data());
	ImGui::Text(edge.data());
	ImGui::Text(face.data());
}

void mesh_component::tick()
{
	//this->render();
	this->renderGUI();
}

void mesh_component::render()
{
	this->mesh->render();
}
