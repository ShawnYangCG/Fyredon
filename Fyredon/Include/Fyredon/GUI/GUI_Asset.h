#pragma once
// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	GUI_Asset.h: Show all asset, Shawn, Niall 
=============================================================================*/
#pragma once

// Project Includes
#include "Fyredon/Other/dllport.h"
#include "Fyredon/GUI/GUI_Panel.h"
#include "Fyredon/ECS/World.h"
#include "Fyredon/GUI/GUIConfig.h"
#include "Fyredon/Render/Texture.h"

// Extern Includes
#include "dearimgui/imgui.h"

#include <vector>
#include <filesystem>

namespace Fyredon
{

class GUI_Asset : public GUI_Panel
{
public:
	GUI_Asset(const char* Title, GUIConfig* guiConfig, World* world);

	virtual void tick() override;
	virtual void resize() override;

protected:
	virtual void draw() override;

	void files_setup();

	World* world;

	GUIConfig* guiConfig;
	int width = 0;
	int height = 0;
	int posY = 0;

private:
	TextureImage* icon_dir; 
	TextureImage* icon_unk_file; 
	
	std::vector<std::filesystem::directory_entry> files;
	std::vector<std::filesystem::directory_entry> dirs;

	std::filesystem::directory_entry* selected; 

	std::filesystem::path path; 
	std::string cur_path;
	bool path_changed; 
};


}
