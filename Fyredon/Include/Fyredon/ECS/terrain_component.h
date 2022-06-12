#pragma once
#include "Fyredon/ECS/Component.h"
#include "Fyredon/ECS/mesh_component.h"
#include "Fyredon/ECS/transform_component.h"
#include "Fyredon/Render/Mesh.h"
#include "Fyredon/ECS/terrain_entity.h"
// 1.
// create your own component by inherit Component class
class terrain_component : public Component
{
public:
	// 2. 
	// rewrite Constructor (give a name to this component)
	terrain_component(terrain_entity* parent);
	// 3.
	// when you create your own component you need to inherit this class and 
	// add your own variable here
	// == variable ==
	Fyredon::Mesh* mesh;
	char fileName = { 0 };
	int res = 512;
	bool flat_normals = false;

	terrain_entity* parent;
	
	// 4.
	// rewrite this function to create your own component GUI
	virtual void GUI_struct() override;
	//virtual void tick() override;
	void recreateMesh();
	//void getMeshDetail();
	//void refreshDetail();
	void setResolution();
	void setFileName();
	//void GUI_struct();
	// virtual void render() override;
private:
	
};
// 5.
// don't forget to add this header in ECS.h
