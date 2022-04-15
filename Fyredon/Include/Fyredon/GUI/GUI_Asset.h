#pragma once
// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	GUI_Asset.h: Show all asset
=============================================================================*/
#pragma once

// Project Includes
#include "Fyredon/Other/dllport.h"
#include "Fyredon/GUI/GUI_Panel.h"
#include "Fyredon/ECS/World.h"
#include "Fyredon/GUI/GUIConfig.h"
// Extern Includes
#include "dearimgui/imgui.h"

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

		World* world;

		const char* title;
		GUIConfig* guiConfig;
		int width = 0;
		int height = 0;
		int posY = 0;
	};
}
