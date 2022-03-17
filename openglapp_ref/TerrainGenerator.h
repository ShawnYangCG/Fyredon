#pragma once
#include <filesystem>
#include <iostream>
#include <vector>
#include "incl/ext/GLEW/glew.h"
#include "mesh.h"
//#include "Fyredon/Extern/stb/stb_image.h"

//using namespace std;

class TerrainGenerator : public Mesh
{
public:
	TerrainGenerator(const char* name, const char* filePath);
	~TerrainGenerator();

	void read_height_map(std::string path);

	std::vector<glm::vec3> vertices;


	void load_obj(bool has_tex = false);

private:

};

