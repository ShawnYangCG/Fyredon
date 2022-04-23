#pragma once
#include "Fyredon/ECS/Component.h"
#include "Fyredon/Render/Mesh.h"
// 1.
// create your own component by inherit Component class
class mesh_component : public Component
{
public:
	// 2. 
	// rewrite Constructor (give a name to this component)
	mesh_component(Fyredon::Primitive* mesh);
	// 3.
	// when you create your own component you need to inherit this class and 
	// add your own variable here
	// == variable ==
	Fyredon::Primitive* mesh;
	// ==============

	// 4.
	// rewrite this function to create your own component GUI
	virtual void GUI_struct() override;
	virtual void tick() override;
	virtual void render() override;
};
// 5.
// don't forget to add this header in ECS.h