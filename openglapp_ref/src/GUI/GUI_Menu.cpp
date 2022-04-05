// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	GUI_Menu.cpp: Top Window GUI Menu
=============================================================================*/

#include "Other/pch.h"

// Implements 
#include "GUI/GUI_Menu.h"

// Extern Includes
#include "imgui.h"

Fyredon::GUI_Menu::GUI_Menu(config menu_config, int32_t y_pad)
	: GUI_Panel(), cfg(menu_config), pad_y(y_pad)
{
	menu_state.about = 0, menu_state.close = 0, menu_state.play = 0; 
	set_title("Fyredon Engine Version 0.0.0");
}

void Fyredon::GUI_Menu::tick()
{
	// Draw Paenl GUI
	draw();

	// Handle Panel State
	if (menu_state.about) about_popout();
}

void Fyredon::GUI_Menu::draw()
{
	main_menu(); 
	play_buttons();
}

// Info : ideally add function to create menu items, but for now, use fixed configs. 
void Fyredon::GUI_Menu::main_menu()
{
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, pad_y));

	switch (cfg)
	{
		case (config::EDIT_VIEW_HELP_ABOUT):
		{
			if (ImGui::BeginMainMenuBar())
			{
				if (ImGui::BeginMenu("Fyredon"))
				{
					if (ImGui::MenuItem("New")) {}
					if (ImGui::MenuItem("Open", "Ctrl+O")) {}
					if (ImGui::MenuItem("Save", "Ctrl+S")) {}
					if (ImGui::MenuItem("Save Project As", "Ctrl+Shift+S")) {}
					ImGui::EndMenu(); // End Fyredon Menu
				}
				if (ImGui::BeginMenu("Edit"))
				{
					if (ImGui::MenuItem("PlaceHolder 1")) {}
					if (ImGui::MenuItem("PlaceHolder 2")) {}
					ImGui::EndMenu(); // End Edit Menu
				}
				if (ImGui::BeginMenu("View"))
				{
					if (ImGui::MenuItem("PlaceHolder 1")) {}
					if (ImGui::MenuItem("PlaceHolder 2")) {}
					ImGui::EndMenu(); // End View Menu
				}
				if (ImGui::BeginMenu("Help"))
				{
					if (ImGui::MenuItem("About")) menu_state.about = 1;
					ImGui::EndMenu(); // End Help Menu
				}

				ImGui::MenuItem(title.c_str(), NULL, false, false);
				ImGui::EndMainMenuBar();
			}
			break;
		}
		case (config::DEBUG):
		{
			if (ImGui::BeginMainMenuBar())
			{
				ImGui::MenuItem((title + " Debug Menu").c_str(), NULL, false, false);
			}
			ImGui::EndMainMenuBar();
		}
	}
	
	ImGui::PopStyleVar(); // frame y_pad 
}

void Fyredon::GUI_Menu::about_popout()
{
	ImGui::Begin("About");
	ImGui::Text("Fyredon Engine\nDoes absolutely nothing currently !");
	ImGui::NewLine();
	if (ImGui::Button("Close"))
	{
		menu_state.about = false;
	}
	ImGui::End();
}

// Pending icon impl 
void Fyredon::GUI_Menu::play_buttons()
{
	return; 
}