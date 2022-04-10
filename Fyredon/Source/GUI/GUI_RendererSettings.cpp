// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	GUI_RendererSettings.cpp: Renderer Settings GUI Panel 
=============================================================================*/
#include "Fyredon/Other/pch.h"
// Implements 
#include "Fyredon/GUI/GUI_RendererSettings.h"

// Extern Includes
#include "dearimgui/imgui.h"

Fyredon::GUI_RendererSettings::GUI_RendererSettings()
	: GUI_Panel()
{
	state.draw_axis = true; 
	state.draw_grid = true; 
}

void Fyredon::GUI_RendererSettings::tick()
{
	draw(); 
}

void Fyredon::GUI_RendererSettings::draw()
{
	const char* t_title = title.c_str(); 
	if (t_title[0] == 0) t_title = "Renderer Settings";
	ImGui::Begin(t_title);

	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(250, 200, 150, 255));
	ImGui::Text("Renderer Settings");
	ImGui::PopStyleColor();
	if (ImGui::Button("Draw Grid")) {
		state.draw_grid = !state.draw_grid;
	}
	if (ImGui::Button("Draw Axis")) {
		state.draw_axis = !state.draw_axis;
	}
	ImGui::End();
}