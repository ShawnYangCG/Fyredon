#include "Fyredon/Other/pch.h"
#include "Fyredon/ECS/camera_component.h"

camera_component::camera_component(Fyredon::Camera* camera)
{
	this->componentName = "Camera";
	this->camera = camera;
	this->outlook = new Fyredon::Primitive("camera");
	float data[176] =
	{
		0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f, 1.f, 0.f, 0.f,
		.5f, .5f, .5f, 0.f, 0.f, 0.f, 1.f, 1.f, 1.f, 0.f, 0.f,

		0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, -1.f, -1.f, 0.f, 0.f,
		.5f, -.5f, -.5f, 0.f, 0.f, 0.f, 1.f, -1.f, -1.f, 0.f, 0.f,

		0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, -1.f, 1.f, 0.f, 0.f,
		.5f, -.5f, .5f, 0.f, 0.f, 0.f, 1.f, -1.f, 1.f, 0.f, 0.f,

		0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f, -1.f, 0.f, 0.f,
		.5f, .5f, -.5f, 0.f, 0.f, 0.f, 1.f, 1.f, -1.f, 0.f, 0.f,

		.5f, -.5f, .5f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f,
		.5f, .5f, .5f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f,
		
		.5f, .5f, .5f, 0.f, 0.f, 0.f, 0.f, 0.f, -1.f, 0.f, 0.f,
		.5f, .5f, -.5f, 0.f, 0.f, 0.f, 0.f, 0.f, -1.f, 0.f, 0.f,

		.5f, .5f, -.5f, 0.f, 0.f, 0.f, 0.f, -1.f, 0.f, 0.f, 0.f,
		.5f, -.5f, -.5f, 0.f, 0.f, 0.f, 0.f, -1.f, 0.f, 0.f, 0.f,

		.5f, -.5f, -.5f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f,
		.5f, -.5f, .5f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f,
	};
	this->outlook->set_data_mesh(data, 16);
	this->outlook->scale(glm::vec3(1.f));
	this->outlook->translate(glm::vec3(0.f, 0.f, 0.f));

	this->outlook->set_shader("..//Shaders//mesh.vert", "..//Shaders//mesh.frag");
	this->outlook->set_colour(glm::vec3(0.8f, 0.8f, 0.8f));

	this->outlook->mode = Fyredon::Render_Mode::RENDER_LINES;
	this->outlook_render = (Fyredon::Primitive*)malloc(sizeof(Fyredon::Primitive));
	memcpy((void*)this->outlook_render, (void*)this->outlook, sizeof(Fyredon::Primitive));
	Fyredon::Renderer::get_renderer().submit_primitive(this->outlook_render);
	this->memmory_size = sizeof(camera_component);
}

void camera_component::GUI_struct()
{
	std::string target = "target: " + std::to_string(0);
	ImGui::Text(target.data());
}
