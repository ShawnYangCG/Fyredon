// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	GUI_Menu.h: Top Window GUI Menu
=============================================================================*/
#pragma once

#include "GUI_Panel.h"
// include file_dialog ...

namespace Fyredon
{
	// class GUI_Icon

class GUI_Menu : public GUI_Panel
{
public:
	enum config { EDIT_VIEW_HELP_ABOUT = 0, DEBUG };

	GUI_Menu(config menu_config, int32_t y_pad = 2);

	virtual void tick() override;
	virtual void resize() override;

protected:
	virtual void draw() override; 

	void about_popout();
	void main_menu();
	void play_buttons();

private: 
	struct
	{
		int8_t open, save, close : 1; // File menu
		int8_t play, pause, stop : 1; // Play menu
		int8_t about : 1;             // About menu
	} menu_state;

	config cfg; 
	int8_t pad_y; 
};

}