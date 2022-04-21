#pragma once
#include "Fyredon/GUI/GUI_PanelOrgnizer.h"
#include "Fyredon/GUI/GUI_Menu.h"
#include "Fyredon/ECS/World.h"
#include "Fyredon/ECS/terrain_entity.h"

namespace Fyredon
{
	class MenuAction {
	public:
		GUI_PanelOrgnizer* panelOrgnizer;
		World* world;
		GUI_Menu* menuPanel;
		MenuAction(GUI_PanelOrgnizer* panelOrgnizer, World* world);
		void tick();
		void createTerrainEntity();
	};
}