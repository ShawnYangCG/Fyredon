#include "Fyredon/Other/pch.h"
#include "Fyredon/ECS/navigation_component.h"
// the parent of this component must contain mesh_component
navigation_component::navigation_component(Entity* parent)
{
	this->componentName = "Navigation";
	this->parent = parent;
	if (this->parent->getComponentByName("Mesh") != nullptr) {
		this->mesh = ((mesh_component*)this->parent->getComponentByName("Mesh"))->mesh;
		getMeshDetail();
	}
	else {
		std::cout << "Error: parent do not contain entity [Mesh]" << std::endl;
	}
	this->memmory_size = sizeof(navigation_component);

	////////////wall test///////////////
	wall = new Fyredon::Mesh_OBJ("wall", "..//Assets//Mesh//wall.obj");
	wall->load_obj();
	wall->set_shader("..//Shaders//mesh.vert", "..//Shaders//mesh.frag");
	wall->set_colour(glm::vec3(0.0f, 0.0f, 1.0f));
	wall->mode = Fyredon::Render_Mode::RENDER_MESH;
	wall->scale(glm::vec3(1.f, 2.f, 1.f));

}

void navigation_component::GUI_struct()
{
	refreshDetail();
	ImGui::InputFloat("Grid width", &this->grid_width);
	std::string grid_detail_X = "min_X: " + std::to_string(this->min_X_tran) + "  max_X: " + std::to_string(this->max_X_tran);
	ImGui::Text(grid_detail_X.data());
	std::string grid_detail_Z = "min_Z: " + std::to_string(this->min_Z_tran) + "  max_Z: " + std::to_string(this->max_Z_tran);
	ImGui::Text(grid_detail_Z.data());
	std::string width_length = "width: " + std::to_string(this->width_tran) + "  length: " + std::to_string(this->length_tran);
	ImGui::Text(width_length.data());
	std::string mid = "middle_X: " + std::to_string(this->mid_X_tran) + "  middle_Z: " + std::to_string(this->mid_Z_tran);
	ImGui::Text(mid.data());
	std::string grid_nxn = "Grid: " + std::to_string(this->widthCount) + "x" + std::to_string(this->lengthCount);
	ImGui::Text(grid_nxn.data());
	std::string gx = "grid min_X: " + std::to_string(this->min_X_grid) + "  grid max_X: " + std::to_string(this->max_X_grid);
	ImGui::Text(gx.data());
	std::string gz = "grid max_Z: " + std::to_string(this->min_Z_grid) + "  grid max_Z: " + std::to_string(this->max_Z_grid);
	ImGui::Text(gz.data());
	if (ImGui::Button("Create grid"))
	{
		createGrid();
		addWalls();
		walls2renderer();
	}
}

void navigation_component::createGrid()
{
	// set grid width(larger than 0 and smaller than width or length)
	if (this->grid_width < this->min_width) {
		this->grid_width = this->min_width;
	}
	
	float maxWidth = (this->width_tran < this->length_tran) ? this->width_tran : this->length_tran;
	if (this->grid_width > maxWidth) {
		this->grid_width = maxWidth;
	}
	// get middle point
	this->widthCount = 2 * (int)((this->width_tran - this->grid_width) / (2.0f * this->grid_width)) + 3;
	this->lengthCount = 2 * (int)((this->length_tran - this->grid_width) / (2.0f * this->grid_width)) + 3;
	this->min_X_grid = this->mid_X_tran - (this->widthCount - 1) * this->grid_width / 2 - this->grid_width / 2;
	this->max_X_grid = this->mid_X_tran + (this->widthCount - 1) * this->grid_width / 2 + this->grid_width / 2;
	this->min_Z_grid = this->mid_Z_tran - (this->lengthCount - 1) * this->grid_width / 2 - this->grid_width / 2;
	this->max_Z_grid = this->mid_Z_tran + (this->lengthCount - 1) * this->grid_width / 2 + this->grid_width / 2;
	this->width_grid = this->max_X_grid - this->min_X_grid;
	this->length_grid = this->max_Z_grid - this->min_Z_grid;
	this->map = new AStarMap(this->widthCount, this->lengthCount);
}

void navigation_component::getMeshDetail() 
{
	if (this->mesh->vert_data.size() > 0) {
		this->min_X = this->mesh->vert_data[0].pos[0];
		this->max_X = this->mesh->vert_data[0].pos[0];
		this->min_Z = this->mesh->vert_data[0].pos[2];
		this->max_Z = this->mesh->vert_data[0].pos[2];
	}
	for (int i = 0; i < this->mesh->vert_data.size(); i++) {
		if (this->mesh->vert_data[i].pos[0] < this->min_X) {
			this->min_X = this->mesh->vert_data[i].pos[0];
		}
		if (this->mesh->vert_data[i].pos[0] > this->max_X) {
			this->max_X = this->mesh->vert_data[i].pos[0];
		}
		if (this->mesh->vert_data[i].pos[2] < this->min_Z) {
			this->min_Z = this->mesh->vert_data[i].pos[2];
		}
		if (this->mesh->vert_data[i].pos[2] > this->max_Z) {
			this->max_Z = this->mesh->vert_data[i].pos[2];
		}
	}
	this->width = this->max_X - this->min_X;
	this->length = this->max_Z - this->min_Z;
	this->mid_X = this->min_X + this->width / 2;
	this->mid_Z = this->min_Z + this->length / 2;
}

void navigation_component::refreshDetail()
{
	float translateX = ((transform_component*)this->parent->getComponentByName("Transform"))->matrixTranslation[0];
	float translateZ = ((transform_component*)this->parent->getComponentByName("Transform"))->matrixTranslation[2];
	float scaleX = ((transform_component*)this->parent->getComponentByName("Transform"))->matrixScale[0];
	float scaleZ = ((transform_component*)this->parent->getComponentByName("Transform"))->matrixScale[2];
	this->min_X_tran = scaleX * this->min_X;
	this->min_Z_tran = scaleZ * this->min_Z;
	this->max_X_tran = scaleX * this->max_X;
	this->max_Z_tran = scaleZ * this->max_Z;
	this->width_tran = this->max_X_tran - this->min_X_tran;
	this->length_tran = this->max_Z_tran - this->min_Z_tran;
	this->min_X_tran = translateX + this->min_X_tran;
	this->max_X_tran = translateX + this->max_X_tran;
	this->min_Z_tran = translateZ + this->min_Z_tran;
	this->max_Z_tran = translateZ + this->max_Z_tran;
	this->mid_X_tran = this->min_X_tran + this->width_tran / 2;
	this->mid_Z_tran = this->min_Z_tran + this->length_tran / 2;
}

list<glm::vec2> navigation_component::findPath(glm::vec2 src, glm::vec2 dest)
{
	glm::vec2 srcIndex = pos2index(src[0], src[1]);
	glm::vec2 destIndex = pos2index(dest[0], dest[1]);
	list<Pos>* poslist = this->map->find(Pos(srcIndex[0], srcIndex[1]), Pos(destIndex[0], destIndex[1]));
	this->posList = poslist;
	list<glm::vec2> position = Pos2Position(poslist);
	return position;
}

glm::vec2 navigation_component::pos2index(float x, float y)
{
	int xIndex, yIndex;
	float width = this->width_grid;
	int xCount = this->widthCount;
	float length = this->length_grid;
	int yCount = this->lengthCount;
	if (x < this->min_X_grid || x > this->max_X_grid) {
		xIndex = 0;
	}
	else {
		xIndex = (x - this->min_X_grid) / (width / xCount);
	}
	if (y < this->min_Z_grid || x > this->max_Z_grid) {
		yIndex = 0;
	}
	else {
		yIndex = (y - this->min_Z_grid) / (length / yCount);
	}
	glm::vec2 index(xIndex, yIndex);
	return index;
}

list<glm::vec2> navigation_component::Pos2Position(list<Pos>* pos)
{
	list<glm::vec2> position;
	for (Pos p : *pos) {
		position.push_back(Pos2Position(p));
	}
	return position;
}

glm::vec2 navigation_component::Pos2Position(Pos pos)
{
	glm::vec2 pt(
		pos.x * this->grid_width + this->grid_width / 2 + this->min_X_grid,
		pos.y * this->grid_width + this->grid_width / 2 + this->min_Z_grid);
	return pt;
}

void navigation_component::addWall(int x, int y)
{
	this->map->addWall(Pos(x, y));
}

void navigation_component::addWalls()
{
	Pos walls[] = { 
		Pos(12,9), Pos(12,10), Pos(12,11),
		Pos(11,9), Pos(10,9), Pos(9,9),
		Pos(9,10), Pos(9,11), Pos(9,12), Pos(9,13), Pos(9,14),
		Pos(10,14), Pos(11,14), Pos(12,14), Pos(13,14), Pos(14,14),Pos(15,14),
		Pos(15,13), Pos(15,12), Pos(15,11), Pos(15,10), Pos(15,9), Pos(15,8), Pos(15,7),Pos(15,6),
		Pos(14,6), Pos(13,6), Pos(12,6), Pos(11,6), Pos(10,6), Pos(9,6), Pos(8,6), Pos(7,6), Pos(6,6), Pos(5,6),
		Pos(5,7), Pos(5,8), Pos(5,9), Pos(5,10), Pos(5,11), Pos(5,12), Pos(5,13), Pos(5,14), Pos(5,15), Pos(5,16), Pos(5,17), Pos(5,18), Pos(5,19), Pos(5,20)
	};

	for (Pos p : walls) {
		this->walls.push_back(p);
		addWall(p.x, p.y);
	}
}

void navigation_component::walls2renderer()
{
	for (int i = 0; i < walls.size(); i++) {
		glm::vec2 pos = Pos2Position(walls[i]);
		Fyredon::Mesh *newMesh = new Fyredon::Mesh(*wall);
		newMesh->translate(glm::vec3(pos[0], 0.25f, pos[1]));
		Fyredon::Renderer::get_renderer().submit_mesh(newMesh);
	}
}
