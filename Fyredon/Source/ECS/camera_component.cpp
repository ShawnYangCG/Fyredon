// Copyright Fyredon. All Rights Reserved.
/*=============================================================================
	Camera_Component.cpp: ECS Camera Component : Niall, Shawn
=============================================================================*/
#include "Fyredon/Other/pch.h"
// Implements
#include "Fyredon/ECS/camera_component.h"

#include "Fyredon/ECS/camera_entity.h"
#include "Fyredon/ECS/World.h"

#include "Fyredon/Core/Application.h"
#include "Fyredon/Render/Assets.h"

// Info : Initalize Camera Component with default state (to later be modified by user).
Fyredon::camera_component::camera_component(Entity* parent)
	: Component(std::string("Camera"), parent)
{
	// Default Component Name
	this->componentName = "Camera_" + std::to_string(World::get_world().counts.camera_count++);

	// Create Camera class within inital state
	parms.pos[0] = 3.f, parms.pos[1] = 1.f, parms.pos[2] = -1.f; 
	parms.fov = 80.f; 
	parms.aspect = 768.f / 504.f;
	parms.free_look = true; 
	camera = new Fyredon::Camera(glm::vec3(parms.pos[0], parms.pos[1], parms.pos[2]), parms.fov, parms.aspect, parms.free_look);

	// Create Frustum Guide Mesh
	guide_mesh = new Fyredon::Mesh((componentName + "_guide-mesh").c_str(), Fyredon::Mesh_Type::GUIDE_MESH);
	guide_mesh->set_data_mesh(Fyredon::Assets::frustum_verts);
	guide_mesh->set_colour(glm::vec3(0.8f, 0.8f, 0.8f));
	guide_mesh->mode = Fyredon::Render_Mode::RENDER_LINES;

	Fyredon::Renderer::get_renderer().submit_mesh_guide(guide_mesh);
}

Fyredon::camera_component::~camera_component()
{
	if (camera)     delete camera; 
	if (guide_mesh) delete guide_mesh; 
}

// Info : Expose key parms to component GUI
void Fyredon::camera_component::GUI_struct()
{
	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(250, 200, 150, 255));
	ImGui::Text("Camera Controls");
	ImGui::PopStyleColor();

	if (ImGui::SliderFloat3("Position", parms.pos, -1e+06, 1e+06))
	{
		camera->position = glm::vec3(parms.pos[0], parms.pos[1], parms.pos[2]);
		std::cout << "Camera Pos Change \n";
	}
	if(ImGui::SliderFloat("FOV", &parms.fov, 1.f, 180.f))
	{
		camera->fov = parms.fov; 
	}
}

// Info : Update Camera State per tick
void Fyredon::camera_component::tick() 
{
	// Get engine singletons
	auto& app      = Fyredon::EditorApplication::get_instance();
	auto& renderer = Fyredon::Renderer::get_renderer(); 

	// Get Application latest GLFWState to update camera from. 
	camera->update_camera(app.get_ogl_context(), 1.f, app.GLFWState.cam_dt, app.GLFWState.delta_yaw, app.GLFWState.delta_pitch);

	// Update Aspect Ratio if changed (not correct currently based on whole editor app size, not viewport).
	if (renderer.width && renderer.height)
	{
		parms.aspect = float(renderer.width) / renderer.height; 
		camera->aspect_ratio = parms.aspect; 
	}

	// ISSUE : GUI Tick, no ImGui Window In Context ? 
	//Component::tick();
}


void Fyredon::camera_component::set_camera_pos(const glm::vec3& pos)
{
	// Update Camera Pos
	parms.pos[0] = pos.x, parms.pos[1] = pos.y, parms.pos[2] = pos.z;
	camera->position = pos;

	// Update Guide Mesh Pos
	guide_mesh->model = glm::inverse(guide_mesh->model);
	guide_mesh->model = glm::mat4(1.f);
	guide_mesh->translate(pos);
}