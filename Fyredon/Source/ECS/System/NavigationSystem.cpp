#include "Fyredon/Other/pch.h"
#include "Fyredon/ECS/System/NavigationSystem.h"

NavigationSystem::NavigationSystem()
{

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
				glm::vec3 destPosition = getTargetPosition();
				glm::vec2 dest2D(destPosition[0], destPosition[2]);

				navigation_component* navCom = (navigation_component*)this->map->getComponentByName("Navigation");
				if (glm::length(dest2D - src2D) != 0) {	// if not arrive the destination
					// if destination change, find path. else goto destnation
					if (isDestinationChange(destPosition)) {
						navCom->path = navCom->findPath(src2D, dest2D);
					}
					else {
						// check whether arrive the finall grid with destination in it
						// if yes, go to destination
						// if no, go to finall grid
						if (navCom->path.size() != 0) {	// no 
							glm::vec2 nextPoint = navCom->path.front();
							// check whether step into next grid
							// if yes, pop the next point from the path list
							// if no, goto next point
							if (isStepIntoGrid(src2D, nextPoint)) { // yes
								navCom->path.pop_front();
								// after we pop the front grid we directly move to the next grid
								// so that to make the movement smoother or it will seems like drop frame
								if (navCom->path.size() != 0) {	
									nextPoint = navCom->path.front();
									glm::vec3 dest3D(nextPoint[0], srcPosition[1], nextPoint[1]);
									((test_move_component*)entity->getComponentByName("Move test"))->moveOneStep(dest3D, this->deltaTime);
								}
								else {
									glm::vec3 dest3D(dest2D[0], srcPosition[1], dest2D[1]);
									((test_move_component*)entity->getComponentByName("Move test"))->moveOneStep(dest3D, this->deltaTime);
								}
							}
							else { // no
								glm::vec3 dest3D(nextPoint[0], srcPosition[1], nextPoint[1]);
								((test_move_component*)entity->getComponentByName("Move test"))->moveOneStep(dest3D, this->deltaTime);
							}
						}else {	// yes
							glm::vec3 dest3D(dest2D[0], srcPosition[1], dest2D[1]);
							((test_move_component*)entity->getComponentByName("Move test"))->moveOneStep(dest3D, this->deltaTime);
						}
					}
				}
			}	
		}
	}
}

void NavigationSystem::addEntity(Entity* entity)
{
	this->entities.push_back(entity);
}

glm::vec3 NavigationSystem::getTargetPosition()
{
	transform_component* transf = (transform_component*)this->destPoint->getComponentByName("Transform");
	glm::vec3 destPosition(transf->matrixTranslation[0], transf->matrixTranslation[1], transf->matrixTranslation[2]);
	return destPosition;
}

bool NavigationSystem::isDestinationChange(glm::vec3 destPosition)
{
	if (this->targetPosition == destPosition)
		return false;
	else {
		this->targetPosition = destPosition;
		return true;
	}
		
}

bool NavigationSystem::isStepIntoGrid(glm::vec2 src, glm::vec2 grid)
{
	navigation_component* navCom = (navigation_component*)this->map->getComponentByName("Navigation");
	glm::vec2 source = navCom->pos2index(src[0], src[1]);
	glm::vec2 gridPos = navCom->pos2index(grid[0], grid[1]);
	if (source[0] == gridPos[0] && source[1] == gridPos[1]) {
		return true;
	}
	return false;
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
		int x = navCom->pos2index(node[0], node[1]).x;
		int y = navCom->pos2index(node[0], node[1]).y;
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

#include <windows.h>
void NavigationSystem::gotoxy(int x, int y)

{

	COORD position;
	position.X = x;
	position.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);

}