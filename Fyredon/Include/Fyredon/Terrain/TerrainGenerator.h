#pragma once
#include <filesystem>
#include <iostream>
#include <vector>
#include "glm/glm.hpp"
#include "Fyredon/Render/Mesh.h"
#include "Fyredon/Debug/Logger.h"


//TODO change namespace

class TerrainGenerator
{
public:
	TerrainGenerator(const char* name, const char* filePath);
	~TerrainGenerator();

	void read_height_map(std::string path);

	float heightFromIndex(int index);

	void draw_water_quad();
	float bilinear_interpolation(float x, float z);


	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;

	void load_obj(bool has_tex = false);

	Fyredon::Mesh TerrainMesh;
	Fyredon::Mesh water_quad;

	int image_width, image_height, image_n_channels;

	float heightFromXZ(int x , int y);

	unsigned char* image_data;
	void generate_mesh(int res);
	float triangle_plane(float x, float z);

private:

};

