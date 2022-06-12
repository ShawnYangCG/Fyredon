#pragma once
#include "Fyredon/ECS/Component.h"
#include "Fyredon/ECS/mesh_component.h"
#include "Fyredon/ECS/transform_component.h"
#include "Fyredon/Render/Mesh.h"
#include "Fyredon/Navigation/AStarFindPath.h"
#include "Fyredon/ECS/terrain_entity.h"
// 1.
// create your own component by inherit Component class
class terrain_component : public Component
{
public:
	// 2. 
	// rewrite Constructor (give a name to this component)
	terrain_component(Entity* parent);
	// 3.
	// when you create your own component you need to inherit this class and 
	// add your own variable here
	// == variable ==
	Fyredon::Mesh* mesh;

	// 4.
	// rewrite this function to create your own component GUI
	virtual void GUI_struct() override;

	//virtual void tick() override;
	void reloadTerrain();
	void getMeshDetail();
	// virtual void render() override;
private:

};
// 5.
// don't forget to add this header in ECS.h
