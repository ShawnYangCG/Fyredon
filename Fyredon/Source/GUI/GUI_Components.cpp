// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	GUI_Components.cpp: Show all components
=============================================================================*/
#include "Fyredon/Other/pch.h"
// Implements 
#include "Fyredon/GUI/GUI_Components.h"

Fyredon::GUI_Components::GUI_Components(const char* Title, GUIConfig* guiConfig, World* world)
	: GUI_Panel(), title(Title), guiConfig(guiConfig), world(world)
{

}

void Fyredon::GUI_Components::tick()
{
	draw();
}

void Fyredon::GUI_Components::resize()
{
	// entitys layout
	int width = this->guiConfig->AppWidth * this->guiConfig->rightBar_width_percent;
	if (width > this->guiConfig->rightBar_width_max)
		width = this->guiConfig->rightBar_width_max;
	if (width < this->guiConfig->rightBar_width_min)
		width = this->guiConfig->rightBar_width_min;
	int height = this->guiConfig->AppHeight * this->guiConfig->rightBar_height_percent;
	//if (height > this->guiConfig->leftBar_height_max)
		//height = this->guiConfig->leftBar_height_max;
	if (height < this->guiConfig->rightBar_height_min)
		height = this->guiConfig->rightBar_height_min;

	this->guiConfig->rightBar_width = width;
	this->width = width;
	this->guiConfig->rightBar_height = height;
	this->height = height;
	this->posX = this->guiConfig->AppWidth - width;
	this->posY = this->guiConfig->menuHeight;
}

void Fyredon::GUI_Components::draw()
{
	resize();
	ImGui::SetNextWindowPos(ImVec2(this->posX, this->posY));
	ImGui::SetNextWindowSize(ImVec2(this->width, this->height));

	ImGui::Begin(title);
	{
		if (this->world->gui_org->gui_panels["Hierarchy"]) {
			Entity* entity = nullptr;
			if (((GUI_EntityList*)this->world->gui_org->gui_panels["Hierarchy"])->getSelectedEntity()) {
				entity = ((GUI_EntityList*)this->world->gui_org->gui_panels["Hierarchy"])->getSelectedEntity();
			}
			if (entity) {
				entity->render();
			}
			if (this->selectedEntity != entity) {
				this->selectedEntity = entity;
				if (actionButton)free(actionButton);
				actionButton = new AddComponentButtonAction(entity);
			}
			if (this->actionButton) {
				this->actionButton->render();
			}
		}
	}
	ImGui::End();
}