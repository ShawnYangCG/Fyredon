#pragma once
#include <map>
#include "Fyredon/GUI/GUI_Panel.h"
class GUI_PanelOrgnizer {
public:
	std::map<std::string, Fyredon::GUI_Panel*> gui_panels;
};
