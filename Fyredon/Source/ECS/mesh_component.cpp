#include "Fyredon/Other/pch.h"
#include "Fyredon/ECS/mesh_component.h"

mesh_component::mesh_component(Fyredon::Mesh* mesh)
{
	this->componentName = "Mesh";
	this->mesh = mesh;
	this->mesh_render = new Fyredon::Mesh(*mesh);
	Fyredon::Renderer::get_renderer().submit_mesh(this->mesh_render);
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
}

void mesh_component::tick()
{
	//this->render();
	
}

void mesh_component::render()
{
	this->renderGUI();
	this->mesh_render->render();
}

void mesh_component::changeMesh(Fyredon::Mesh* mesh, bool IsReplace = false)
{
	if (IsReplace)
		free(this->mesh);
	this->mesh = mesh;
	if (this->mesh_render)free(this->mesh_render);
	this->mesh_render = new Fyredon::Mesh(*mesh);
}
