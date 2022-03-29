// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	GUI_FloatingWidget.h: Floating Dialog/Widget
=============================================================================*/
#pragma once

// Project Includes
#include "Fyredon/Other/dllport.h"
#include "Fyredon/Core/GUI_Panel.h"

// Extern Includes
#include "dearimgui/imgui.h"

namespace Fyredon
{ 
class GUI_FloatingWidget : public GUI_Panel
{
public:
	GUI_FloatingWidget();

	virtual void tick() override; 

protected:
	virtual void draw() override; 

	struct 
	{
		float slider_val; 
		bool button_state; 
	} state;

	ImVec2 slider_range;
};
}