// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	GUI_Menu.h: Top Window GUI Menu : Niall, Shawn
=============================================================================*/
#pragma once

#include "GUI_Panel.h"
#include "Fyredon/GUI/GUIConfig.h"
#include "Fyredon/GUI/GUI_NewProjectWindow.h"
// include file_dialog ...

namespace Fyredon
{
	// class GUI_Icon

class GUI_Menu : public GUI_Panel
{
public:
	enum config { EDIT_VIEW_HELP_ABOUT = 0, DEBUG };

	GUI_Menu(config menu_config, GUIConfig* gui_config, int32_t y_pad = 2);

	virtual void tick() override;
	virtual void resize() override;

protected:
	virtual void draw() override; 

	void about_popout();
	void main_menu();
	void play_buttons();
	void saved_state();

private: 
	config cfg; 
	int8_t pad_y; 
	GUIConfig* gui_config;

	// new project
	GUI_NewProjectWindow* newProject;
	bool isNext = false;

public:
	struct
	{
		int8_t IsNew, open, save, saveAs : 1; // File menu
		int8_t play, pause, reset : 1; // Play menu
		int8_t about : 1;             // About menu
		int8_t savestate : 1;		  // Save state
		int8_t createTerrain : 1;	  // GameObject menu
	} menu_state;
	void new_project();
};

}