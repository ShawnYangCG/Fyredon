// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	GUI_FloatingWidget.cpp: Floating Dialog/Widget
=============================================================================*/
#include "Fyredon/Other/pch.h"
// Implements 
#include "Fyredon/GUI/GUI_FloatingWidget.h"

// Extern Includes
#include "dearimgui/imgui.h"

Fyredon::GUI_FloatingWidget::GUI_FloatingWidget(const char *Title)
	: GUI_Panel(), title(Title)
{
	state.slider_val = 0.f; 
	slider_range.x = 0.f, slider_range.y = 10.f; 
	state.button_state = false; 
}

void Fyredon::GUI_FloatingWidget::tick()
{
	draw(); 
}

void Fyredon::GUI_FloatingWidget::resize()
{
}

void Fyredon::GUI_FloatingWidget::draw()
{
	ImGui::Begin(title);

	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(250, 200, 150, 255));
	ImGui::Text("Floating Panel");
	ImGui::PopStyleColor();

	ImGui::Text("Stand in text");
	//ImGui::SliderFloat("Slider", &state.slider_val, slider_range.x, slider_range.y);
	//if (ImGui::Button("Button")) state.button_state = !state.button_state;
	ImGui::End();
}