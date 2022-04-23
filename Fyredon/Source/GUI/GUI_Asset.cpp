// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	GUI_Asset.cpp: Show all asset, Shawn, Niall
=============================================================================*/
#include "Fyredon/Other/pch.h"
// Implements 
#include "Fyredon/GUI/GUI_Asset.h"

// Extern Includes
#include "dearimgui/imgui.h"

#include <cstring>
#include <thread>

#define ICON_SIZE 64

// Add Title to GUI_Panel base ctor.
Fyredon::GUI_Asset::GUI_Asset(const char* Title, GUIConfig* guiConfig, World* world)
	: GUI_Panel(Title), guiConfig(guiConfig), world(world), path_changed(false)
{
	// Default Path
	cur_path = std::filesystem::current_path().string();
	
	// Icon Setup
	icon_dir = new TextureImage("..//Assets//Images//Icons//open-folder.png", "dir_icon", 0, false);
	icon_unk_file = new TextureImage("..//Assets//Images//Icons//unknown-icon.png", "unk_file_icon", 0, false);
	icon_dir->load(), icon_unk_file->load();
}

void Fyredon::GUI_Asset::tick()
{
	files_setup();
	draw();
}

void Fyredon::GUI_Asset::resize()
{
	// entitys layout
	int width = this->guiConfig->AppWidth;
	int yheight = this->guiConfig->leftBar_height > this->guiConfig->rightBar_height ?
		this->guiConfig->leftBar_height : this->guiConfig->rightBar_height;
	int height = this->guiConfig->AppHeight - this->guiConfig->menuHeight - yheight;
	this->width = width;
	this->height = height;
	this->posY = this->guiConfig->menuHeight + yheight;
}

void Fyredon::GUI_Asset::draw()
{
	resize();

	ImGui::SetNextWindowPos(ImVec2(0, this->posY));
	ImGui::SetNextWindowSize(ImVec2(this->width, this->height));

	static char loc_path [512];
	strcpy_s(loc_path, 256, cur_path.c_str());

	std::size_t total = files.size() + dirs.size();
	std:size_t rows   = total / 6; 

	ImGui::Begin(title.c_str());
	{
		// Directory (local path) String
		if (ImGui::InputText("Directory", loc_path, 512))
		{
			cur_path = loc_path;
			path_changed = true;
		}

		if (total > 0)
		{
			ImGui::Columns(12, 0, false); // Begin Column Layout

			// Parent Dir
			//ImGui::PushID(1);
			if (ImGui::ImageButton(icon_dir->get_imgui_texture(), ImVec2(ICON_SIZE, ICON_SIZE)))
			{
				path_changed = true;
				cur_path = std::filesystem::path(cur_path).parent_path().string();
			}
			ImGui::Text("..");
			ImGui::NextColumn();

			// Dirs
			for (std::filesystem::directory_entry& dir : dirs)
			{
				// Dir Button
				if (ImGui::ImageButton(icon_dir->get_imgui_texture(), ImVec2(ICON_SIZE, ICON_SIZE)))
				{
					path_changed = true;
					cur_path = dir.path().string();
				}
				// Dir Label
				ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));
				ImGui::Text(dir.path().filename().string().c_str());
				ImGui::PopStyleColor();
				ImGui::NextColumn();
			}

			// Files
			for (std::filesystem::directory_entry& file : files)
			{
				// File Button
				if (ImGui::ImageButton(icon_unk_file->get_imgui_texture(), ImVec2(ICON_SIZE, ICON_SIZE)))
				{
					// File Selected
					selected = &file; 
				}
				// File Label
				ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));
				ImGui::TextWrapped(file.path().filename().string().c_str());
				ImGui::PopStyleColor();
				ImGui::NextColumn();
			} 

			ImGui::Columns(1); // End Column layout
		}

		// Dir Changed, re-draw in dir
		if (path_changed) 
		{
			path_changed = false; 
			files_setup();
			draw();
		}
	}
	ImGui::End();
}

void Fyredon::GUI_Asset::files_setup()
{
	files.clear(), dirs.clear();
	path = std::filesystem::path(cur_path);

	if (!std::filesystem::exists(path)) return;
	
	for (const auto& file : std::filesystem::directory_iterator(path)) 
	{
		if (file.is_directory()) dirs.push_back(file);
		else files.push_back(file);
	}
}