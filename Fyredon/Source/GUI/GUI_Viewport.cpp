#include "Fyredon/Other/pch.h"
// Implements
#include "Fyredon/GUI/GUI_Viewport.h"

// Extern Includes
#include "dearimgui/imgui.h"

Fyredon::GUI_Viewport::GUI_Viewport(const char* Title, GUIConfig* guiConfig) 
	: GUI_Panel(), title(Title), guiConfig(guiConfig)
{
}

void Fyredon::GUI_Viewport::tick()
{
	draw();
}

void Fyredon::GUI_Viewport::resize()
{
	int width = guiConfig->AppWidth - guiConfig->leftBar_width - guiConfig->rightBar_width;
	int yheight = guiConfig->leftBar_height > guiConfig->rightBar_height ?
		guiConfig->leftBar_height : guiConfig->rightBar_height;
	int height = yheight;
	this->width = width;
	this->height = height;
	this->posY = this->guiConfig->menuHeight;
	this->posX = this->guiConfig->leftBar_width;
}

void Fyredon::GUI_Viewport::draw()
{
	resize();
	ImGui::SetNextWindowPos(ImVec2(this->posX, this->posY));
	ImGui::SetNextWindowSize(ImVec2(this->width, this->height));

	Fyredon::Texture* tex = Renderer::get_renderer().get_colour_attachment();

	ImGui::Begin("Game Viewport");
	if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
	{
		if (ImGui::BeginTabItem(" Scene "))
		{
			// Flip V Min. 
			ImVec2 uv_min = ImVec2(0.0f, 1.0f);
			ImVec2 uv_max = ImVec2(1.0f, 0.0f); // flip.
			ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
			ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f);
			ImGui::Image(tex->get_imgui_texture(), ImVec2(tex->width, tex->height), uv_min, uv_max, tint_col, border_col);
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem(" Game "))
		{
			
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::End();
}
