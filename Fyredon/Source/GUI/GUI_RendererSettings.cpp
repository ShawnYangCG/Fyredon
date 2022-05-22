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
	static float grid_scale = 1.f; 
	static float grid_tile = 2.f; 

	// ======= ImGui Renderer Draw =======
	ImGui::Begin("Renderer Settings");

	// Renderer - Viewer Settings
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(250, 200, 150, 255));
	ImGui::Text("Viewer Settings");
	ImGui::PopStyleColor();

	if (ImGui::SliderFloat("Grid_Size", &grid_scale, 1.f, 10.f))
	{
		renderer.viewer_prims.ground->set_size(grid_scale);
		renderer.viewer_prims.ground->set_tile(grid_scale / 1.f);
	}
	ImGui::Checkbox("Draw Grid", &renderer.parms.draw_grid);

	ImGui::Checkbox("Draw Axis", &renderer.parms.draw_axis);

	ImGui::Checkbox("Draw Test Mesh", &renderer.parms.draw_test_mesh);

	ImGui::Checkbox("Draw Guide Meshes", &renderer.parms.draw_mesh_guides);
		
	// =========== Renderer - Light Settings ===========
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(250, 200, 150, 255));
	ImGui::Text("Light Settings");
	ImGui::PopStyleColor();

	// =========== Renderer - Light Settings - Sun
	if (ImGui::Checkbox("Enable Sun", &renderer.parms.enable_sun)) {
		renderer.rebuild_material_shaders();
	}
		
	if (ImGui::SliderFloat("Sun Strength", &renderer.parms.sun_strength, 0.f, 10.f)) {
		// No need to rebuild for str
	}

	if (ImGui::SliderFloat3("Sun_Colour", sun_col, 0.f, 1.f)) {
		renderer.parms.sun_colour.x = sun_col[0];
		renderer.parms.sun_colour.y = sun_col[1];
		renderer.parms.sun_colour.z = sun_col[2];
	}

	if (ImGui::SliderFloat3("Sun_Direction", sun_dir, -1.f, 1.f)) {
		renderer.parms.sun_direction.x = sun_dir[0];
		renderer.parms.sun_direction.y = sun_dir[1];
		renderer.parms.sun_direction.z = sun_dir[2];
	}

	if (ImGui::Checkbox("Sun Shadows", &renderer.parms.sun_shadows)) {
		renderer.rebuild_material_shaders();
	}
		
	// Renderer - Culling Settings
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(250, 200, 150, 255));
	ImGui::Text("Culling Settings");
	ImGui::PopStyleColor();

	if (ImGui::Checkbox("Cull Backface", &renderer.parms.cull_backface)) {
		/*renderer.parms.cull_backface = !renderer.parms.cull_backface;*/
	} 

	// Renderer - PostProcess Settings
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(250, 200, 150, 255));
	ImGui::Text("Post Processing Settings");
	ImGui::PopStyleColor();
	ImGui::Text("ToDo");

	ImGui::End();
}