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

class TerrainGenerator
{
public:
	TerrainGenerator(const char* name, const char* filePath);
	~TerrainGenerator();

	void read_height_map(std::string path);

	float heightFromIndex(int index);

	void draw_water_quad();
	float bilinear_interpolation(float x, float z);
	float heightFromXZ(int x, int y);

	void generate_mesh(int res);
	float triangle_plane(float x, float z);
	void load_terrain_image(std::string path);
	void write_obj(std::string path);
	void smooth_normals();
	void load_obj(bool has_tex = false);


	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;

	int terrain_res;

	Fyredon::Mesh TerrainMesh;
	Fyredon::Mesh water_quad;
	Fyredon::Mesh tree1;

	int image_width, image_height, image_n_channels;


	int getUp(int index, int res);
	int getDown(int index, int res);
	int getRight(int index, int res);
	int getLeft(int index, int res);


	//Fyredon::TextureImage grass;
	unsigned char* image_data;

	std::vector<unsigned char> image_data_sample;
	
	bool terrain_loaded = false;

	float terrain_height_scale = 0.02;
	float terrain_coverage_size = 0.1;


	void load_terrain_tree(std::string path);
private:

};

