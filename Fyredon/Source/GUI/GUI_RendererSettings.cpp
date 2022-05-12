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
{}

void Fyredon::GUI_RendererSettings::tick()
{
	draw(); 
}

void Fyredon::GUI_RendererSettings::resize()
{
	// 
}

// Info : Set Renederer Singleton Parameters via this GUI Panel. 
void Fyredon::GUI_RendererSettings::draw()
{
	Renderer& renderer = Renderer::get_renderer();

	// Temporary GUI state
	static float sun_dir[3] = {renderer.parms.sun_direction.x, renderer.parms.sun_direction.y, renderer.parms.sun_direction.z};
	static float sun_col[3] = {renderer.parms.sun_colour.x,    renderer.parms.sun_colour.y,    renderer.parms.sun_colour.z   };

	// ======= ImGui Renderer Draw =======
	ImGui::Begin("Renderer Settings");

	// Renderer - Viewer Settings
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(250, 200, 150, 255));
	ImGui::Text("Viewer Settings");
	ImGui::PopStyleColor();

	if (ImGui::Button("Draw Grid")) renderer.parms.draw_grid = !renderer.parms.draw_grid;

	if (ImGui::Button("Draw Axis")) renderer.parms.draw_axis = !renderer.parms.draw_axis; 

	if (ImGui::Button("Draw Test Mesh")) renderer.parms.draw_test_mesh = !renderer.parms.draw_test_mesh;
		
	// Renderer - Light Settings 
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(250, 200, 150, 255));
	ImGui::Text("Light Settings");
	ImGui::PopStyleColor();

	// Renderer - Light Settings - Sun
	if (ImGui::Button("Enable Sun")) renderer.parms.enable_sun = !renderer.parms.enable_sun;

	ImGui::SliderFloat("Sun Strength", &renderer.parms.sun_strength, 0.f, 10.f);

	if (ImGui::SliderFloat3("Sun_Colour", sun_col, 0.f, 1.f))
	{
		renderer.parms.sun_colour.x = sun_col[0];
		renderer.parms.sun_colour.y = sun_col[1];
		renderer.parms.sun_colour.z = sun_col[2];
	}

	if (ImGui::SliderFloat3("Sun_Direction", sun_dir, -1.f, 1.f))
	{
		renderer.parms.sun_direction.x = sun_dir[0];
		renderer.parms.sun_direction.y = sun_dir[1];
		renderer.parms.sun_direction.z = sun_dir[2];
	}

	if (ImGui::Button("Sun Shadows")) renderer.parms.sun_shadows = !renderer.parms.sun_shadows;

	// Renderer - Culling Settings
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(250, 200, 150, 255));
	ImGui::Text("Culling Settings");
	ImGui::PopStyleColor();

	if (ImGui::Checkbox("Cull Backface", &renderer.parms.cull_backface)) {} /*renderer.parms.cull_backface = !renderer.parms.cull_backface;*/

	// Renderer - PostProcess Settings
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(250, 200, 150, 255));
	ImGui::Text("Post Processing Settings");
	ImGui::PopStyleColor();
	ImGui::Text("ToDo");

	ImGui::End();
}