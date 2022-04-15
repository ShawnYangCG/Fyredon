// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	GUI_EntityList.cpp: Show all entities
=============================================================================*/
#include "Fyredon/Other/pch.h"
// Implements 
#include "Fyredon/GUI/GUI_EntityList.h"

// Extern Includes
#include "dearimgui/imgui.h"

Fyredon::GUI_EntityList::GUI_EntityList(const char* Title, GUIConfig* guiConfig, World* world)
	: GUI_Panel(), title(Title), guiConfig(guiConfig), world(world)
{

}

int Fyredon::GUI_EntityList::getSelectedID()
{
	return this->selectedEntityIndex;
}

void Fyredon::GUI_EntityList::tick()
{
	draw();
}

void Fyredon::GUI_EntityList::resize()
{
	int width = this->guiConfig->AppWidth * this->guiConfig->leftBar_width_percent;
	if (width > this->guiConfig->leftBar_width_max)
		width = this->guiConfig->leftBar_width_max;
	if (width < this->guiConfig->leftBar_width_min)
		width = this->guiConfig->leftBar_width_min;
	int height = this->guiConfig->AppHeight * this->guiConfig->leftBar_height_percent;
	//if (height > this->guiConfig->leftBar_height_max)
		//height = this->guiConfig->leftBar_height_max;
	if (height < this->guiConfig->leftBar_height_min)
		height = this->guiConfig->leftBar_height_min;

	this->guiConfig->leftBar_width = width;
	this->width = width;
	this->guiConfig->leftBar_height = height;
	this->height = height;
	this->posY = this->guiConfig->menuHeight;
}

void Fyredon::GUI_EntityList::draw()
{
	// entitys layout
	resize();

	ImGui::SetNextWindowPos(ImVec2(0, this->posY));
	ImGui::SetNextWindowSize(ImVec2(this->width, this->height));

	ImGui::Begin(title);
	{
		
		/*
		for (int i = 0; i < this->world->entities.size(); i++) {
			char label[128];
			sprintf_s(label, "%s", this->world->entities[i]->getName().data());
			if (ImGui::Selectable(label, this->selectedEntityIndex == i))
				this->selectedEntityIndex = i;
		}*/
	}
	ImGui::End();
}