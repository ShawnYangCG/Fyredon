#include "ECS/Component.h"

Component::Component()
{
	this->componentName = "new component";
}

Component::Component(std::string componentName)
{
	this->componentName = componentName;
}

void Component::renderGUI()
{
	if (ImGui::CollapsingHeader(this->componentName.data())) {
		GUI_struct();
	}
}

// this function is the structure of GUI, if you want to design your own GUI for
// the component, you need to rewrite this function
void Component::GUI_struct()
{
}
