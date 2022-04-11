#include "ECS/System/NavigationSystem.h"

NavigationSystem::NavigationSystem(World* world)
{
	this->world = world;

	wall = new Mesh("wall", "./wall.obj");
	wall->load_obj(false);
	wall->set_shader("../../shaders/mesh4.vert", "../../shaders/mesh4.frag");
	wall->set_colour(glm::vec3(0.0f, 0.0f, 1.0f));
	wall->mode = Render_Mode::RENDER_MESH;
	wall->scale(glm::vec3(1.f, 2.f, 1.f));
}

void NavigationSystem::tick()
{
	clock_t currentTime = clock();
	if (float(currentTime - this->lastTime) / CLOCKS_PER_SEC >= deltaTime) {
		this->lastTime = currentTime;
		for (Entity* entity : this->entities) {
			if (entity->getComponentByName("Move test") != nullptr) {
				// get source position
				transform_component* transfSrc = (transform_component*)entity->getComponentByName("Transform");
				glm::vec3 srcPosition(transfSrc->matrixTranslation[0], transfSrc->matrixTranslation[1], transfSrc->matrixTranslation[2]);
				glm::vec2 src2D(srcPosition[0], srcPosition[2]);

				// get destination position
				transform_component* transf = (transform_component*)this->destPoint->getComponentByName("Transform");
				glm::vec3 destPosition(transf->matrixTranslation[0], transf->matrixTranslation[1], transf->matrixTranslation[2]);
				glm::vec2 dest2D(destPosition[0], destPosition[2]);
				
				// find path
				if (glm::length(dest2D - src2D) != 0) {
					navigation_component* navCom = (navigation_component*)this->map->getComponentByName("Navigation");
					list<glm::vec2> path = navCom->findPath(src2D, dest2D);
					//printMap(path);
					if (path.size() != 0) path.pop_back();
					if (path.size() != 0) path.pop_front();
					
					path.push_back(dest2D);
					glm::vec2 nextDest = path.front();
					glm::vec3 dest3D(nextDest[0], srcPosition[1], nextDest[1]);
					((test_move_component*)entity->getComponentByName("Move test"))->moveOneStep(dest3D, this->deltaTime);
				}
			}	
		}
	}
}

void NavigationSystem::addEntity(Entity* entity)
{
	this->entities.push_back(entity);
}

void NavigationSystem::printMap(list<glm::vec2> path) {
	navigation_component* navCom = (navigation_component*)this->map->getComponentByName("Navigation");
	char* charmap = (char*)malloc(sizeof(char) * navCom->widthCount * navCom->lengthCount);
	
	for (int i = 0; i < navCom->lengthCount; i++) {
		for (int j = 0; j < navCom->widthCount;j++) {
			charmap[i * navCom->widthCount + j] = '.';
		}
	}
	for (glm::vec2 node : path) {
		int x = navCom->pos2index(node[0], node[1], navCom->width_grid, navCom->widthCount, navCom->length_grid, navCom->lengthCount).x;
		int y = navCom->pos2index(node[0], node[1], navCom->width_grid, navCom->widthCount, navCom->length_grid, navCom->lengthCount).y;
		charmap[y * navCom->widthCount + x] = '#';
	}
	gotoxy(0, 127);
	std::cout << "===============================" << std::endl;
	for (int i = 0; i < navCom->lengthCount; i++) {
		for (int j = 0; j < navCom->widthCount; j++) {
			std::cout << " " << charmap[i * navCom->widthCount + j];
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void NavigationSystem::randerWalls()
{
	navigation_component* navCom = (navigation_component*)this->map->getComponentByName("Navigation");

	for (int i = 0; i < navCom->walls.size(); i++) {
		glm::vec2 pos = navCom->Pos2Position(navCom->walls[i]);

		wall->translate(glm::vec3(pos[0], 0.25f, pos[1]));
		this->enrenSys->renderMesh(wall);
		wall->translate(glm::vec3(-pos[0], -0.25f, -pos[1]));
	}
}

#include <windows.h>
void NavigationSystem::gotoxy(int x, int y)

{

	COORD position;
	position.X = x;
	position.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);

}