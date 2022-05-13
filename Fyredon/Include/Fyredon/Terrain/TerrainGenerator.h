#pragma once

#include <filesystem>
#include <iostream>
#include <vector>

// External
#include "glm/glm.hpp"
#include "stb/stb_image.h"

// Fyredon headers
#include "Fyredon/Render/Mesh.h"
#include "Fyredon/Debug/Logger.h"
#include "Fyredon/Render/Texture.h"

//TODO change namespace

// Old (Need to merge Izzy's latest, this still uses old heightmaps)

class TerrainGenerator
{
public:
	TerrainGenerator(const char* name, const char* filePath);
	~TerrainGenerator();

	void read_height_map(std::string path);

	float heightFromIndex(int index);

	float bilinear_interpolation(float x, float z);

	float heightFromXZ(int x, int y);

	void generate_mesh(int res);

	float triangle_plane(float x, float z);

	void write_obj(std::string path);

	void smooth_normals();

	void load_obj(bool has_tex = false);

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;

	Fyredon::Mesh *TerrainMesh;

	int image_width, image_height, image_n_channels;
	unsigned char* image_data;

	int terrain_res;
	
	bool terrain_loaded = false;
};

