#include "Fyredon/Other/pch.h"
#include "Fyredon/ECS/Component.h"

Component::Component()
{
	this->componentName = "new component";
	this->memmory_size = sizeof(Component);
}

Component::Component(std::string componentName)
{
	this->componentName = componentName;
	this->memmory_size = sizeof(Component);
}

Component::~Component() = default;

void Component::init()
{
    // A pause/resume belongs to the same simulation session.
    if (!hasSnapshot) {
        initialActive = isActive;
        hasSnapshot = true;
    }
    isRunning = true;
}

void Component::reset()
{
    // Never byte-copy polymorphic objects, strings, containers or resource pointers.
    if (hasSnapshot) isActive = initialActive;
    hasSnapshot = false;
    isRunning = false;
}
void Component::tick()
{
	//render();
	
}

void Component::play()
{
}

void Component::render()
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

int Component::getSize()
{
	return this->memmory_size;
}

void Component::setRunning(bool isRunning)
{
	this->isRunning = isRunning;
}
