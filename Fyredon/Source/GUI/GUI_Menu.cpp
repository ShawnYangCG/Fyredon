// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	GUI_Menu.cpp: Top Window GUI Menu : Niall, Shawn
=============================================================================*/

#include "Fyredon/Other/pch.h"

// Implements 
#include "Fyredon/GUI/GUI_Menu.h"

// Extern Includes
#include "dearimgui/imgui.h"
#include "Fyredon/Core/SaveState.h"
#include "Fyredon/GUI/GUI_RendererSettings.h"

Fyredon::GUI_Menu::GUI_Menu(config menu_config, GUIConfig* gui_config, int32_t y_pad)
	: GUI_Panel(), cfg(menu_config), pad_y(y_pad) ,gui_config(gui_config)
{
	menu_state.IsNew = 0, menu_state.open = 0, menu_state.save = 0, menu_state.saveAs = 0;
	menu_state.play = 0, menu_state.pause = 0, menu_state.reset = 0;
	menu_state.about = 0,
	menu_state.createTerrain = 0, menu_state.pigHead = 0, menu_state.createDoll = 0, menu_state.createCamera = 0, menu_state.tree1 = 0;
	menu_state.createNavigationDemo = 0;
	newProject = new GUI_NewProjectWindow(this->gui_config->AppWidth, this->gui_config->AppHeight);
	set_title("Fyredon Engine Version 0.0.0");
}

Fyredon::GUI_Menu::~GUI_Menu()
{
	if (newProject)free(newProject);
}



void Fyredon::GUI_Menu::tick()
{
	// Draw Paenl GUI
	draw();

	// Handle Panel State
	if (menu_state.about) about_popout();

	if (menu_state.savestate) {
		saved_state();
		menu_state.savestate = false;
	}
	if (menu_state.loadstate) {
		loaded_state();
		menu_state.loadstate = false;
	}
}

void Fyredon::GUI_Menu::resize()
{
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
					if (ImGui::MenuItem("New")) {
						this->menu_state.IsNew = true;
					}
					if (ImGui::MenuItem("Open", "Ctrl+O")) {}
					if (ImGui::MenuItem("Save", "Ctrl+S")) {}
					if (ImGui::MenuItem("Save Project As", "Ctrl+Shift+S")) {}
					ImGui::EndMenu(); // End Fyredon Menu
				}
				if (ImGui::BeginMenu("Edit"))
				{
					if (ImGui::MenuItem("Save State")) { menu_state.savestate = true; }
					if (ImGui::MenuItem("Load State")) { menu_state.loadstate = true; }
					ImGui::EndMenu(); // End Edit Menu
				}
				if (ImGui::BeginMenu("GameObject"))
				{
					if (ImGui::MenuItem("Terrain")) { menu_state.createTerrain = true; }
					if (ImGui::BeginMenu("3D Object"))
					{
						if (ImGui::MenuItem("Pig Head")) { menu_state.pigHead = true; }
						if (ImGui::MenuItem("Tree1")) { menu_state.tree1 = true; }
						ImGui::EndMenu();
					}
					if (ImGui::MenuItem("Doll")) { menu_state.createDoll = true; }
					if (ImGui::MenuItem("Camera")) { menu_state.createCamera = true; }
					if (ImGui::MenuItem("Navigation Demo")) { menu_state.createNavigationDemo = true; }
					if (ImGui::MenuItem("PlaceHolder 2")) {}
					ImGui::EndMenu(); // End Edit Menu
				}
				if (ImGui::BeginMenu("View"))
				{
					if (ImGui::MenuItem("Renderer Settings")) { menu_state.view_renderer = true; }
					if (ImGui::MenuItem("PlaceHolder 2")) {}
					ImGui::EndMenu(); // End View Menu
				}
				if (ImGui::BeginMenu("Help"))
				{
					if (ImGui::MenuItem("About")) menu_state.about = 1;
					ImGui::EndMenu(); // End Help Menu
				}


				ImGui::MenuItem(title.c_str(), NULL, false, false);
				std::string space = "";
				for (int i = 0; i < (this->gui_config->AppWidth / 2 - 495 - 45 - 20) / 8; i++)
					space += " ";
				ImGui::MenuItem(space.data(), NULL, false, false);
				if (menu_state.play == false) {
					ImGui::PushStyleColor(ImGuiCol_Header, IM_COL32(0, 150, 0, 255));
					if (ImGui::MenuItem("Play", NULL, !menu_state.play, true)) {
						menu_state.play = true;
						menu_state.pause = false;
						//======== Play ========

						//======================
					}
					ImGui::PopStyleColor();
				}
				else {
					ImGui::PushStyleColor(ImGuiCol_Header, IM_COL32(200, 0, 0, 255));
					if (ImGui::MenuItem("Pause", NULL, menu_state.play, true)) {
						menu_state.play = false;
						menu_state.pause = true;
					}
					ImGui::PopStyleColor();
				}

				ImGui::PushStyleColor(ImGuiCol_Header, IM_COL32(0, 0, 200, 255));
				if (ImGui::MenuItem("Reset", NULL, true, menu_state.pause)) {
					menu_state.play = false;
					menu_state.pause = false;
					//======== Reset ========
					menu_state.reset = true;
					//=======================
				}
				ImGui::PopStyleColor();

				ImGui::EndMainMenuBar();
			}
			break;
			//play_buttons();
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
	ImGui::Text("Fyredon Engine\n Team Fyredon @ University Of Leeds\n MSc HPG 21-22");
	ImGui::NewLine();
	if (ImGui::Button("Close")) {
		menu_state.about = false;
	}
	ImGui::End();
}

// Pending icon impl 
void Fyredon::GUI_Menu::play_buttons()
{
	
}

void Fyredon::GUI_Menu::new_project()
{
	if (!isNext) {
		newProject->reLocation(this->gui_config->AppWidth,this->gui_config->AppHeight);
		newProject->tick();
		this->isNext = newProject->getIsNext();
	}
	else {
		this->menu_state.IsNew = false;
		this->isNext = false;
		newProject->resetState();
	}
}

// Save engine state called
void Fyredon::GUI_Menu::saved_state()
{
	SaveState::getInstance().update = true;
}

// Load engine state called
void Fyredon::GUI_Menu::loaded_state()
{
	SaveState::getInstance().loaded = true;
}