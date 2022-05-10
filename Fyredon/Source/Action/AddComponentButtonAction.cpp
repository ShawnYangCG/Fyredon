#include "Fyredon/Other/pch.h"
#include "Fyredon/Action/AddComponentButtonAction.h"

AddComponentButtonAction::AddComponentButtonAction(Entity* parent)
{
	this->parent = parent;
}

void AddComponentButtonAction::tick()
{
	if (ImGui::Button("add component"))
	{
		this->isClicked = true;
	}
	if (this->isClicked) {
		ImGui::BeginChild("child", ImVec2(0, 100), true);
		{
			if (ImGui::Selectable("Transform Component", this->selectedComponent == TransformComponent))
				this->selectedComponent = TransformComponent;

			if (ImGui::Selectable("Mesh Component", this->selectedComponent == MeshComponent))
				this->selectedComponent = MeshComponent;

			if (ImGui::Selectable("Navigation Component", this->selectedComponent == NavigationComponent))
				this->selectedComponent = NavigationComponent;

			if (ImGui::Selectable("Script Component", this->selectedComponent == ScriptComponent))
				this->selectedComponent = ScriptComponent;
		}
		ImGui::EndChild();
		if (ImGui::Button("add"))
		{
			if(this->selectedComponent != -1)
				addComponent(this->selectedComponent);
			this->selectedComponent = -1;
			this->isClicked = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("close"))
		{
			this->selectedComponent = -1;
			this->isClicked = false;
		}
	}
}

void AddComponentButtonAction::addComponent(int ALL_COMPONENTS)
{
	if (ALL_COMPONENTS == TransformComponent) {
		this->parent->addTransformComponent();
	}
	if (ALL_COMPONENTS == MeshComponent) {
		//this->parent->addComponent();
	}
	if (ALL_COMPONENTS == NavigationComponent) {
		navigation_component* navCom = new navigation_component(this->parent);
		this->parent->addComponent(navCom);
	}
	if (ALL_COMPONENTS == ScriptComponent) {
		//this->parent->addComponent();
	}
}
