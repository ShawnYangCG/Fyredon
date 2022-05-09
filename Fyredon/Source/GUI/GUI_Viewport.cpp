// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	GUI_Viewport.h: Base Engine Editor Application : Shawn, Niall
=============================================================================*/
#include "Fyredon/Other/pch.h"
// Implements
#include "Fyredon/GUI/GUI_Viewport.h"

// Project Includes
#include "Fyredon/ECS/World.h"
#include "Fyredon/ECS/camera_component.h"
#include "Fyredon/ECS/camera_entity.h"

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

// Info : Set Camera based on Viewport tab selected, renderer will render to this cam on next tick. 
void Fyredon::GUI_Viewport::draw()
{
	resize();
	ImGui::SetNextWindowPos(ImVec2(this->posX, this->posY));
	ImGui::SetNextWindowSize(ImVec2(this->width, this->height));

	Fyredon::Renderer &renderer = Renderer::get_renderer();
	World& world = World::get_world();

	Fyredon::Texture* tex = renderer.get_colour_attachment();

	ImGui::Begin("Game Viewport");
	if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
	{
		if (ImGui::BeginTabItem(" Scene "))
		{
			// Set Viewport and Camera to Scene_Cam
			current_viewport = viewport::VIEWPORT_SCENE;
			if (world.game_cam_entity)
			{
				camera_component* game_cam = world.game_cam_entity->get_camera_component();
				renderer.set_active_camera(game_cam->camera);
			}

			// Get Rendered View Texture
			ImVec2 uv_min = ImVec2(0.0f, 1.0f);
			ImVec2 uv_max = ImVec2(1.0f, 0.0f); // flip.
			ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
			ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f);
			ImGui::Image(tex->get_imgui_texture(), ImVec2(tex->width, tex->height), uv_min, uv_max, tint_col, border_col);
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem(" Game "))
		{
			// Set Viewport and Camera to Game_Cam
			current_viewport = viewport::VIEWPORT_GAME;
			if (world.scene_cam_entity)
			{
				camera_component* scene_cam = world.scene_cam_entity->get_camera_component();
				renderer.set_active_camera(scene_cam->camera);
			}

			// Get Rendered View Texture
			ImVec2 uv_min = ImVec2(0.0f, 1.0f);
			ImVec2 uv_max = ImVec2(1.0f, 0.0f); // flip.
			ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
			ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f);
			ImGui::Image(tex->get_imgui_texture(), ImVec2(tex->width, tex->height), uv_min, uv_max, tint_col, border_col);

			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::End();
}
