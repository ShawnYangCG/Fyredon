#include "Fyredon/Other/pch.h"
#include "Fyredon/ECS/camera_component.h"

camera_component::camera_component(Fyredon::Camera* camera)
{
	this->componentName = "Camera";
	this->camera = camera;

	this->outlook = new Fyredon::Mesh("camera");
	using Fyredon::Vert;
	std::vector<Vert> data =
	{
		Vert(0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f, 1.f, 0.f, 0.f, 0.f),
		Vert(.5f, .5f, .5f, 0.f, 0.f, 0.f, 1.f, 1.f, 1.f, 0.f, 0.f, 0.f),
		Vert(0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, -1.f, -1.f, 0.f, 0.f, 0.f),
		Vert(.5f, -.5f, -.5f, 0.f, 0.f, 0.f, 1.f, -1.f, -1.f, 0.f, 0.f, 0.f),
		Vert(0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, -1.f, 1.f, 0.f, 0.f, 0.f),
		Vert(.5f, -.5f, .5f, 0.f, 0.f, 0.f, 1.f, -1.f, 1.f, 0.f, 0.f, 0.f),
		Vert(0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f, -1.f, 0.f, 0.f, 0.f),
		Vert(.5f, .5f, -.5f, 0.f, 0.f, 0.f, 1.f, 1.f, -1.f, 0.f, 0.f, 0.f),
		Vert(.5f, -.5f, .5f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f),
		Vert(.5f, .5f, .5f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f),
		Vert(.5f, .5f, .5f, 0.f, 0.f, 0.f, 0.f, 0.f, -1.f, 0.f, 0.f, 0.f),
		Vert(.5f, .5f, -.5f, 0.f, 0.f, 0.f, 0.f, 0.f, -1.f, 0.f, 0.f, 0.f),
		Vert(.5f, .5f, -.5f, 0.f, 0.f, 0.f, 0.f, -1.f, 0.f, 0.f, 0.f, 0.f),
		Vert(.5f, -.5f, -.5f, 0.f, 0.f, 0.f, 0.f, -1.f, 0.f, 0.f, 0.f, 0.f),
		Vert(.5f, -.5f, -.5f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f),
		Vert(.5f, -.5f, .5f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f)
	};
	outlook->set_data_mesh(data);
	outlook->set_shader("..//Shaders//guides.vert", "..//Shaders//guides.frag");
	outlook->set_colour(glm::vec3(0.8f, 0.8f, 0.8f));
	outlook->scale(glm::vec3(1.f));
	outlook->translate(glm::vec3(0.f, 0.f, 0.f));
	outlook->mode = Fyredon::Render_Mode::RENDER_LINES;

	outlook_render = new Fyredon::Mesh(*outlook);
	Fyredon::Renderer::get_renderer().submit_mesh(outlook_render);

	// Note : This does not store correct memory size, doesn't count heap allocations. 
	this->memmory_size = sizeof(camera_component);
}

void camera_component::GUI_struct()
{
	std::string target = "target: " + std::to_string(0);
	ImGui::Text(target.data());
}
