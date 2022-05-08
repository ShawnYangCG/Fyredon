#include "Fyredon/Other/pch.h"
#include "Fyredon/ECS/Component.h"

Component::Component()
{
	this->componentName = "default new component";
	this->memmory_size = sizeof(Component);
	this->parent = nullptr; 
}

Component::Component(std::string componentName, Entity* parent)
{
	this->componentName = componentName;
	this->memmory_size = sizeof(Component);
	this->parent = parent; 
}

Component::~Component()
{
	if (componentBackup) delete componentBackup;
}

void Component::init()
{
	componentBackup = new Component(*this);
}

void Component::reset()
{
	//memcpy((void*)this, (void*)this->componentBackup, this->memmory_size);
	if (componentBackup != nullptr) *this = *componentBackup;
}

void Component::tick()
{
	renderGUI();
}

void Component::stop() {
	if (this->isRunning == true) {
		this->isRunning = false;
	}
}

void Component::renderGUI()
{
	if (ImGui::CollapsingHeader(this->componentName.data())) {
		// the lable of check box can not be same so we need to use underbar "_" between the name and the componentName
		// the component name will not show in GUI if we use underbar naming method like below
		ImGui::Checkbox(("Active_"+this->componentName).data(), &this->isActive);
		GUI_struct();
	}
}

// this function is the structure of GUI, if you want to design your own GUI for
// the component, you need to rewrite this function
void Component::GUI_struct()
{
}

void Component::setRunning(bool isRunning)
{
	this->isRunning = isRunning;
}
