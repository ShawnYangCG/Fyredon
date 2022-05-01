#include "Fyredon/Other/pch.h"
#include "Fyredon/ECS/mesh_component.h"

mesh_component::mesh_component(Fyredon::Mesh* mesh)
{
	this->componentName = "Mesh";
	this->mesh = mesh;
	Fyredon::Mesh* mesh2 = (Fyredon::Mesh*)malloc(sizeof(Fyredon::Mesh));
	memcpy((void*)mesh2, (void*)this->mesh, sizeof(Fyredon::Mesh));
	this->mesh_render = mesh2;
	Fyredon::Renderer::get_renderer().submit_primitive(this->mesh_render);
	this->memmory_size = sizeof(mesh_component);
}

void mesh_component::GUI_struct()
{
	std::string vertex = "Vertex: " + std::to_string(this->mesh->vert_data.size());
	
	std::string edge = "Edge:";
	std::string face = "Face:";
	ImGui::Text(vertex.data());
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
	this->mesh_render->render();
}
