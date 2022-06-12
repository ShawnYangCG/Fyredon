#include "Fyredon/Other/pch.h"
#include "Fyredon/Action/AddComponentButtonAction.h"

AddComponentButtonAction::AddComponentButtonAction(Entity* parent)
{
	this->parent = parent;
}

AddComponentButtonAction::~AddComponentButtonAction()
{
	if (this->parent) {
		this->parent->~Entity();
		if (this->parent)free(this->parent);
	}
}

void AddComponentButtonAction::tick()
{
	
}

void AddComponentButtonAction::render()
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
			if (this->selectedComponent != -1)
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
		this->parent->addComponent();
	}
	if (ALL_COMPONENTS == NavigationComponent) {
		
		this->parent->addComponent();
	}
	if (ALL_COMPONENTS == ScriptComponent) {
		Component* navigation = new test_move_component();
		this->parent->addComponent(navigation);
	}
}
