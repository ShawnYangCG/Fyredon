#pragma once
#include "Fyredon/ECS/Component.h"
#include "Fyredon/Render/Mesh.h"
#include "Fyredon/Render/Renderer.h"
// 1.
// create your own component by inherit Component class
class mesh_component : public Component
{
public:
	// 2. 
	// rewrite Constructor (give a name to this component)
	mesh_component(Fyredon::Mesh* mesh);
	~mesh_component();
	// 3.
	// when you create your own component you need to inherit this class and 
	// add your own variable here
	// == variable ==
	Fyredon::Mesh* mesh;
	Fyredon::Mesh* mesh_render;
	// ==============

	// 4.
	// rewrite this function to create your own component GUI
	virtual void GUI_struct() override;
	virtual void tick() override;
	virtual void render() override;
	void changeMesh(Fyredon::Mesh* mesh, bool IsReplace);
};
// 5.
// don't forget to add this header in ECS.h