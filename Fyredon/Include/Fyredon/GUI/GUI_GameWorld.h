// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	GUI_GameWorld.h: Floating Dialog/Widget
=============================================================================*/
#pragma once

// Project Includes
#include "Fyredon/Other/dllport.h"
#include "Fyredon/GUI/GUI_Panel.h"

// Extern Includes
#include "dearimgui/imgui.h"

namespace Fyredon
{ 
class GUI_GameWorld : public GUI_Panel
{
public:
	GUI_GameWorld();

	virtual void tick() override; 

protected:
	virtual void draw() override; 
};

}