#pragma once
#include "Fyredon/ECS/Entity.h"
#include "Fyredon/ECS/transform_component.h"
#include "Fyredon/ECS/test_move_component.h"
// Extern Includes
#include "dearimgui/imgui.h"
class Entity;
class AddComponentButtonAction
{
public:
	enum {
		TransformComponent,
		MeshComponent,
		NavigationComponent,
		ScriptComponent
	} ALL_COMPONENTS;
	Entity* parent;
	AddComponentButtonAction(Entity* parent);
	~AddComponentButtonAction();
	void tick();
	void render();
	void addComponent(int ALL_COMPONENTS);
private:
	int selectedComponent = -1;
	bool isClicked = false;

};

