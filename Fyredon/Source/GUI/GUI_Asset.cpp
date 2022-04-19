// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	GUI_Asset.cpp: Show all asset
=============================================================================*/
#include "Fyredon/Other/pch.h"
// Implements 
#include "Fyredon/GUI/GUI_Asset.h"

// Extern Includes
#include "dearimgui/imgui.h"

#include <filesystem>

#define ICON_SIZE 64

// Add Title to GUI_Panel base ctor.
Fyredon::GUI_Asset::GUI_Asset(const char* Title, GUIConfig* guiConfig, World* world)
	: GUI_Panel(Title), guiConfig(guiConfig), world(world)
{
	// Icon Setup
	icon_dir = new TextureImage("..//Assets//Images//Icons//open-folder.png", "dir_icon", 0, false);
	icon_unk_file = new TextureImage("..//Assets//Images//Icons//unkown-file.png", "unk_file_icon", 0, false);
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

	static const char* loc_path;
	std::size_t total = files.size() + dirs.size();
	std:size_t rows   = total / 6; 

	ImGui::Begin(title.c_str());
	{
		if (total > 0)
		{
			// Draw Dirs first
			for (std::size_t d = 0; d < dirs.size(); ++d)
			{
				if (d % 6 == 0) ImGui::NewLine();
				ImGui::ImageButton(icon_dir->get_imgui_texture(), ImVec2(ICON_SIZE, ICON_SIZE));
			}

			/*
			for (const auto& dir : dirs)
			{
				//ImGui::ImageButton(icon_dir->get_imgui_texture(), ImVec2(ICON_SIZE, ICON_SIZE));
			} */

			//ImGui::ImageButton(icon_dir->get_imgui_texture(), ImVec2(ICON_SIZE, ICON_SIZE));
		}
	}
	ImGui::End();
}

void Fyredon::GUI_Asset::files_setup()
{
	files.clear(), dirs.clear();
	std::filesystem::path path = std::filesystem::current_path();
	std::cout << path << "\n";
	if (path.empty() || !std::filesystem::exists(path)) return;

	for (const auto& file : std::filesystem::directory_iterator(path)) 
	{
		if (file.is_directory()) dirs.push_back(file);
		else files.push_back(file);
	}
}