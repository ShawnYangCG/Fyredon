#include "Fyredon/Other/pch.h"
#include "Fyredon/GUI/GUI_PanelOrgnizer.h"

GUI_PanelOrgnizer::GUI_PanelOrgnizer()
{
}

GUI_PanelOrgnizer::~GUI_PanelOrgnizer()
{
	while (!gui_panels.empty()){
		auto iter = gui_panels.begin();
		if (iter->second) {
			iter->second->~GUI_Panel();
			free(iter->second);
		}
		gui_panels.erase(iter);
	}
}

std::size_t GUI_PanelOrgnizer::get_panel_count()
{
	return gui_panels.size();
}
