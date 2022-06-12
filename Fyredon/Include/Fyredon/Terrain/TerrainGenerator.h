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
#include <Fyredon/Render/Mesh_OBJ.h>

//TODO change namespace

class TerrainGenerator
{
public:
	TerrainGenerator(const char* name, const char* filePath);
	~TerrainGenerator();

	void load_terrain_image(std::string path);

	void read_height_map(std::string path);
	void generate_mesh(int res);
	void smooth_normals();


	float bilinear_interpolation(float x, float z);
	float triangle_plane(float x, float z);
	float heightFromXZ(int x, int y);
	float heightFromIndex(int index);

	void write_obj(std::string path);
	void load_obj(bool has_tex = false);

	int getUp(int index, int res);
	int getDown(int index, int res);
	int getRight(int index, int res);
	int getLeft(int index, int res);

	void load_terrain_tree(std::string path);

	void draw_water_quad();

	int terrain_res;
	int image_width, image_height, image_n_channels;

	Fyredon::Mesh *TerrainMesh;
	Fyredon::Mesh *water_quad;
	Fyredon::Mesh_OBJ *tree1;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;


	unsigned char* image_data;
	std::vector<unsigned char> image_data_sample;

	bool terrain_loaded = false;

	float terrain_height_scale = 0.02;
	float terrain_coverage_size = 0.1;

	bool flat_normals = false;


private:

};

