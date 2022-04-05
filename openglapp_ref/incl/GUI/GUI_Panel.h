// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	GUI_Panel.h: Base Engine Editor Application
=============================================================================*/
#pragma once

// Project Includes
#include <Other/dllport.h>

#include <string>
//#include <functional>

namespace Fyredon
{ 
class GUI_Panel
{
public:
	GUI_Panel() : title("\0") {}
	virtual ~GUI_Panel() = default;

	void set_title(const char* str) { title = str; }

	virtual void tick() = 0;

protected:
	virtual void draw() = 0;

	//std::function<void> clicked_callback;
	std::string title; 
};
}