#pragma once
#include <filesystem>
#include <iostream>
#include <vector>
#include "glm/glm.hpp"
#include "Fyredon/Render/Mesh.h"
#include "Fyredon/Debug/Logger.h"

class TerrainGenerator
{
public:
	TerrainGenerator(const char* name, const char* filePath);
	~TerrainGenerator();

	void read_height_map(std::string path);

	float heightFromIndex(int index);

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;

	void load_obj(bool has_tex = false);

	Fyredon::Mesh TerrainMesh;

private:

};

