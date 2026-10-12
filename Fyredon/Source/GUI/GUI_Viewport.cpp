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

	auto& renderer = Renderer::get_renderer();
	if (!ImGui::IsMouseDown(2)) camera_dragging = false;

	ImGui::Begin("Game Viewport", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
	if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
	{
		if (ImGui::BeginTabItem(" Scene "))
		{
			// Match the render target to the drawable area, excluding tabs and padding.
			const ImVec2 available = ImGui::GetContentRegionAvail();
			const int renderWidth = static_cast<int>(available.x) - 2;
			const int renderHeight = static_cast<int>(available.y) - 2;
			if (renderWidth > 0 && renderHeight > 0) {
				if (renderer.width != renderWidth || renderer.height != renderHeight) {
					renderer.resize(renderWidth, renderHeight);
					renderer.render(); // Fill the new texture before ImGui displays it.
				}
				auto* tex = renderer.get_colour_attachment();
				// Flip V Min. 
				ImVec2 uv_min = ImVec2(0.0f, 1.0f);
				ImVec2 uv_max = ImVec2(1.0f, 0.0f); // flip.
				ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
				ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f);
				ImGui::Image(tex->get_imgui_texture(), ImVec2(tex->width, tex->height), uv_min, uv_max, tint_col, border_col);
				const bool hovered = ImGui::IsItemHovered();
				const bool beginDrag = hovered && ImGui::IsMouseClicked(2);
				if (beginDrag) camera_dragging = true;
				const auto& io = ImGui::GetIO();
				const float dx = camera_dragging && !beginDrag ? io.MouseDelta.x : 0.f;
				const float dy = camera_dragging && !beginDrag ? io.MouseDelta.y : 0.f;
				const float wheel = hovered ? io.MouseWheel : 0.f;
				if (renderer.game_cam && (dx != 0.f || dy != 0.f || wheel != 0.f))
					renderer.game_cam->update_camera(dx, dy, wheel, io.KeyShift);
				}
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
