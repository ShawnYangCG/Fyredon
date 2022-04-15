// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	GUI_EntityList.h: Show all entities
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
	class GUI_EntityList : public GUI_Panel
	{
	public:
		GUI_EntityList(const char* Title, GUIConfig* guiConfig, World* world);

		Entity* getSelectedEntity();

		virtual void tick() override;
		virtual void resize() override;

	protected:
		virtual void draw() override;
		void printChild(int depth, Entity* entity);
		
		World* world;
		Entity* selectedEntity = nullptr;

		const char* title;
		GUIConfig* guiConfig;
		int width = 0;
		int height = 0;
		int posY = 0;
	};
}