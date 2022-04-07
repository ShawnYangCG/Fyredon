#pragma once
#include <filesystem>
#include <iostream>
#include <vector>
#include "../GLEW/glew.h"
#include "mesh.h"
//#include "Fyredon/Extern/stb/stb_image.h"

//using namespace std;

class TerrainGenerator : public Mesh
{
public:
	TerrainGenerator(const char* name, const char* filePath);
	~TerrainGenerator();

	void read_height_map(std::string path);

	float heightFromIndex(int index);

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;



	void load_obj(bool has_tex = false);

private:

};

