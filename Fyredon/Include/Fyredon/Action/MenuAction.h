#pragma once
#include "Fyredon/GUI/GUI_PanelOrgnizer.h"
#include "Fyredon/GUI/GUI_Menu.h"
//#include "Fyredon/ECS/World.h"
//#include "Fyredon/ECS/terrain_entity.h"
//#include "Fyredon/ECS/camera_entity.h"
#include "Fyredon/ECS/ECS.h"
//#include "Fyredon/ECS/fbx_entity.h"

namespace Fyredon
{
	class MenuAction {
	public:
		GUI_PanelOrgnizer* panelOrgnizer;
		World* world;
		GUI_Menu* menuPanel;
		MenuAction(GUI_PanelOrgnizer* panelOrgnizer, World* world);
		~MenuAction();
		void tick();
		void createTerrainEntity();
		void createPigHead();
		void createTree1();
		void createDoll();
		void createCamera();
		void createNavigationDemo();
		void render_settings();
	};
}