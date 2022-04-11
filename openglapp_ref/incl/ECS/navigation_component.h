#pragma once
#include "ECS/Component.h"
#include "ECS/mesh_component.h"
#include "ECS/transform_component.h"
#include "mesh.h"
#include "Navigation/AStarFindPath.hpp"
// 1.
// create your own component by inherit Component class
class navigation_component : public Component
{
public:
	// 2. 
	// rewrite Constructor (give a name to this component)
	navigation_component(Entity* parent);
	// 3.
	// when you create your own component you need to inherit this class and 
	// add your own variable here
	// == variable ==
	Mesh* mesh;
	AStarMap* map;
	std::vector<Pos> walls;
	float min_width = 0.5;
	list<Pos>* posList;
	
	float grid_width;
	float min_X_tran, max_X_tran, min_Z_tran, max_Z_tran;
	float mid_X_tran, mid_Z_tran;
	float width_tran, length_tran;
	int widthCount = 0;
	int lengthCount = 0;
	// ==============
	float min_X, max_X, min_Z, max_Z;
	float mid_X, mid_Z;
	float width, length;

	float min_X_grid = 0, max_X_grid = 0, min_Z_grid = 0, max_Z_grid = 0;
	float width_grid = 0, length_grid = 0;

	// 4.
	// rewrite this function to create your own component GUI
	virtual void GUI_struct() override;
	//virtual void tick() override;
	void createGrid();
	void getMeshDetail();
	void refreshDetail();
	list<glm::vec2> findPath(glm::vec2 src, glm::vec2 dest);
	Pos pos2index(float x, float y, float width, int xCount, float length, int yCount);
	list<glm::vec2> Pos2Position(list<Pos>* pos);
	glm::vec2 Pos2Position(Pos pos);
	void addWall(int x, int y);
	void addWalls();
	// virtual void render() override;
private:
	
};
// 5.
// don't forget to add this header in ECS.h
