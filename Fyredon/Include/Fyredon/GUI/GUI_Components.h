#pragma once
// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	GUI_Components.h: Show all components
=============================================================================*/
#pragma once

// Project Includes
#include "Fyredon/Other/dllport.h"
#include "Fyredon/GUI/GUI_Panel.h"
#include "Fyredon/ECS/World.h"
#include "Fyredon/GUI/GUIConfig.h"
#include "Fyredon/GUI/GUI_EntityList.h"
#include "Fyredon/Action/AddComponentButtonAction.h"
// Extern Includes
#include "dearimgui/imgui.h"

namespace Fyredon
{
	class GUI_Components : public GUI_Panel
	{
	public:
		GUI_Components(const char* Title, GUIConfig* guiConfig, World* world);

		virtual void tick() override;
		virtual void resize() override;

	protected:
		virtual void draw() override;

		World* world;

		const char* title;
		GUIConfig* guiConfig;
		int width = 0;
		int height = 0;
		int posX = 0;
		int posY = 0;

		Entity* selectedEntity = nullptr;
		AddComponentButtonAction* actionButton = nullptr;
	};
}
