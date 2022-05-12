#include "Fyredon/Other/pch.h"
#include "Fyredon/ECS/mesh_component.h"

mesh_component::mesh_component(Fyredon::Mesh* mesh)
{
	this->componentName = "Mesh";
	this->mesh = mesh;
	Fyredon::Renderer::get_renderer().submit_mesh(this->mesh);
	this->memmory_size = sizeof(mesh_component);
}

// Remove mesh from render_list if component deleted
mesh_component::mesh_component::~mesh_component()
{
	//auto& ren = Fyredon::Renderer::get_renderer();
	//ren.delete_mesh ...;
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