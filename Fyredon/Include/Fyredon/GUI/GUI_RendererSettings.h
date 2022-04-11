// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	GUI_RendererSettings.h: Renderer Settings GUI Panel: Niall 
=============================================================================*/
#pragma once

// Project Includes
#include "Fyredon/Other/dllport.h"
#include "Fyredon/GUI/GUI_Panel.h"

// Extern Includes
#include "dearimgui/imgui.h"

namespace Fyredon
{ 
class GUI_RendererSettings : public GUI_Panel
{
public:
	GUI_RendererSettings();

	virtual void tick() override; 

	struct {
		bool draw_grid;
		bool draw_axis;
		bool draw_testmesh; 
	} state;

protected:
	virtual void draw() override; 

};

}