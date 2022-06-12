// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	GUI_Panel.h: Base Engine Editor Application: Niall
=============================================================================*/
#pragma once

// Project Includes
#include <Fyredon/Other/dllport.h>
#include <string>

namespace Fyredon
{ 
// Info : Base class for all GUI Panels to implement

class GUI_Panel
{
public:
	GUI_Panel() : title("\0") {}
	GUI_Panel(const char *Title) : title(Title) {}
	virtual ~GUI_Panel() = default;

	void set_title(const char* str) { title = str; }

	virtual void tick() = 0;
	virtual void resize() = 0;

protected:
	virtual void draw() = 0;

	std::string title; 
};
}