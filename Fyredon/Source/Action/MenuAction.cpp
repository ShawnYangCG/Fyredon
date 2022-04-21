#include "Fyredon/Other/pch.h"
#include "Fyredon/Action/MenuAction.h"

Fyredon::MenuAction::MenuAction(GUI_PanelOrgnizer* panelOrgnizer, World* world)
{
	this->world = world;
	this->panelOrgnizer = panelOrgnizer;
	this->menuPanel = (GUI_Menu*)panelOrgnizer->gui_panels["Menu"];
}

void Fyredon::MenuAction::tick()
{
	if (menuPanel->menu_state.IsNew) {
		menuPanel->new_project();
	}
	if (menuPanel->menu_state.save) {
		menuPanel->menu_state.save = false;
	}
	if (menuPanel->menu_state.createTerrain) {
		createTerrainEntity();
		menuPanel->menu_state.createTerrain = false;
	}
}

void Fyredon::MenuAction::createTerrainEntity()
{
	terrain_entity* entity = new terrain_entity();
	entity->setName(entity->getName() + "_terrain");
	this->world->addEntity(entity);
}
