// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	GUI_GameWorld.cpp: Floating Dialog/Widget
=============================================================================*/
#include "Fyredon/Other/pch.h"
// Implements 
#include "Fyredon/GUI/GUI_GameWorld.h"

// Extern Includes
#include "dearimgui/imgui.h"

#include "Fyredon/Core/GameWorld.h"

Fyredon::GUI_GameWorld::GUI_GameWorld()
	: GUI_Panel()
{
}

void Fyredon::GUI_GameWorld::tick()
{
	draw(); 
}

void Fyredon::GUI_GameWorld::draw()
{
	GameWorld world = GameWorld::get_world();
	
	ImGui::Begin("Gane World");

	// State Creation
	if (ImGui::Button("Create Sphere"))
	{
		auto s = world.prims.size();
		world.create_mesh_obj((std::string("Sphere_") + std::to_string(s)).c_str(), "..//Assets//Mesh//sphere.obj");
	}

	ImGui::Dummy(ImVec2(0.0f, 10.0f));

	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(250, 200, 150, 255));
	ImGui::Text("Primitives");
	ImGui::PopStyleColor();

	// List Prims
	for ( const auto& [key, value] : world.prims_map)
	{
		ImGui::Text(key.c_str());
	}
	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(250, 200, 150, 255));
	ImGui::Text("Lights");
	ImGui::PopStyleColor();

	// List Lights
	for (const auto& [key, value] : world.lights_map)
	{
		ImGui::Text(key.c_str());
	}
	ImGui::End();
}