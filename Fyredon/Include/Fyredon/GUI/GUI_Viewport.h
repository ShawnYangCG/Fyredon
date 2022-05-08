// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	GUI_Viewport.h: Base Engine Editor Application
=============================================================================*/
#pragma once
#include "GUI_Panel.h"
#include "Fyredon/GUI/GUIConfig.h"
#include "Fyredon/Render/Texture.h"
#include "Fyredon/Render/Renderer.h"


namespace Fyredon
{
// class GUI_Icon
class GUI_Viewport : public GUI_Panel
{
public:
	enum config { EDIT_VIEW_HELP_ABOUT = 0, DEBUG };
	enum viewport {VIEWPORT_SCENE = 0, VIEWPORT_GAME};
	GUI_Viewport(const char* Title, GUIConfig* guiConfig);

	virtual void tick() override;
	virtual void resize() override;

	viewport current_viewport = viewport::VIEWPORT_SCENE; 
protected:
	virtual void draw() override;
	const char* title;

protected:
	GUIConfig* guiConfig;
	int width = 0;
	int height = 0;
	int posX = 0;
	int posY = 0;
};

}