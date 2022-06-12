// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	GUI_RendererSettings.cpp: Renderer Settings GUI Panel 
=============================================================================*/
#include "Fyredon/Other/pch.h"
// Implements 
#include "Fyredon/GUI/GUI_RendererSettings.h"
#include "Fyredon/Render/Renderer.h"

// Extern Includes
#include "dearimgui/imgui.h"

Fyredon::GUI_RendererSettings::GUI_RendererSettings()
	: GUI_Panel()
{
	state.draw_axis     = true; 
	state.draw_grid     = true; 
	state.draw_testmesh = true; 
	state.light_strength = 2.0f;
	state.light_pos = glm::vec3(0.f, 5.f, 3.f);
	state.test_mesh_pos = glm::vec3(0.f, 1.f, 0.f);

	// Hook renderer flags to GUI
	Renderer& renderer = Renderer::get_renderer();
	renderer.flags.draw_axis = &state.draw_axis;
	renderer.flags.draw_grid = &state.draw_grid;
	renderer.flags.draw_test_prims = &state.draw_testmesh;
	renderer.flags.light_pos = &state.light_pos;
	renderer.flags.light_str = &state.light_strength;
	renderer.flags.test_pos = &state.test_mesh_pos;
}

void Fyredon::GUI_RendererSettings::tick()
{
	// Pre-Processing of state...

	// Draw GUI
	draw(); 
}

void Fyredon::GUI_RendererSettings::resize()
{
}

void Fyredon::GUI_RendererSettings::draw()
{
	//ImGui::SetNextWindowPos(ImVec2(800, 20));
	//ImGui::SetNextWindowSize(ImVec2(200, 400));

	static float light_pos[3] = { state.light_pos.x, state.light_pos.y, state.light_pos.z };
	static float test_pos[3] = { state.test_mesh_pos.x, state.test_mesh_pos.y, state.test_mesh_pos.z };

	const char* t_title = title.c_str(); 
	if (t_title[0] == 0) t_title = "Renderer Settings";
	ImGui::Begin(t_title);
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(250, 200, 150, 255));
	ImGui::Text("Viewer Settings");
	ImGui::PopStyleColor();
	if (ImGui::Button("Draw Grid")) {
		state.draw_grid = !state.draw_grid;
	}
	if (ImGui::Button("Draw Axis")) {
		state.draw_axis = !state.draw_axis;
	}
	if (ImGui::Button("Draw Test Mesh")) {
		state.draw_testmesh = !state.draw_testmesh;
	}
	if (ImGui::SliderFloat3("Test Mesh", test_pos, -2.5f, 2.5f))
	{
		state.test_mesh_pos.x = test_pos[0];
		state.test_mesh_pos.y = test_pos[1];
		state.test_mesh_pos.z = test_pos[2];
	}
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(250, 200, 150, 255));
	ImGui::Text("(temp) Light Settings");
	ImGui::PopStyleColor();
	ImGui::SliderFloat("Light Strength", &state.light_strength, 0.f, 10.f);
	if (ImGui::SliderFloat3("Light Position", light_pos, -100.f, 100.f))
	{
		state.light_pos.x = light_pos[0];
		state.light_pos.y = light_pos[1];
		state.light_pos.z = light_pos[2];
	}
	ImGui::End();
}