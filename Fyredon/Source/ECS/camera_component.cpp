#include "Fyredon/Other/pch.h"
#include "Fyredon/ECS/camera_component.h"

camera_component::camera_component(Fyredon::Camera* camera)
{
	componentName = "Camera";
	camera = camera;

	guide_mesh = new Fyredon::Mesh("camera");
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
	guide_mesh->set_data_mesh(data);
	guide_mesh->set_shader("..//Shaders//guides.vert", "..//Shaders//guides.frag");
	guide_mesh->set_colour(glm::vec3(0.8f, 0.8f, 0.8f));
	guide_mesh->scale(glm::vec3(1.f));
	guide_mesh->translate(glm::vec3(0.f, 0.f, 0.f));
	guide_mesh->mode = Fyredon::Render_Mode::RENDER_LINES;

	Fyredon::Renderer::get_renderer().submit_mesh(guide_mesh);
}

void camera_component::GUI_struct()
{
	std::string target = "target: " + std::to_string(0);
	ImGui::Text(target.data());
}
