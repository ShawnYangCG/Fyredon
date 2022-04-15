// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	GUI_Asset.cpp: Show all asset
=============================================================================*/
#include "Fyredon/Other/pch.h"
// Implements 
#include "Fyredon/GUI/GUI_Asset.h"

// Extern Includes
#include "dearimgui/imgui.h"
Fyredon::GUI_Asset::GUI_Asset(const char* Title, GUIConfig* guiConfig, World* world)
	: GUI_Panel(), title(Title), guiConfig(guiConfig), world(world)
{

}

void Fyredon::GUI_Asset::tick()
{
	draw();
}

void Fyredon::GUI_Asset::resize()
{
	// entitys layout
	int width = this->guiConfig->AppWidth;
	int yheight = this->guiConfig->leftBar_height > this->guiConfig->rightBar_height ?
		this->guiConfig->leftBar_height : this->guiConfig->rightBar_height;
	int height = this->guiConfig->AppHeight - this->guiConfig->menuHeight - yheight;
	this->width = width;
	this->height = height;
	this->posY = this->guiConfig->menuHeight + yheight;
}

void Fyredon::GUI_Asset::draw()
{
	resize();

	ImGui::SetNextWindowPos(ImVec2(0, this->posY));
	ImGui::SetNextWindowSize(ImVec2(this->width, this->height));

	ImGui::Begin(title);
	{

	}
	ImGui::End();
}