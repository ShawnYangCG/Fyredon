#include "Fyredon/Other/pch.h"
#include <Fyredon/Core/TerrainGeneration.h>
#include "Fyredon/Extern/GLEW/glew.h"
#define STB_IMAGE_IMPLEMENTATION
#include <Fyredon/Extern/stb/stb_image.h>

TerrainGenerator::TerrainGenerator() {
	read_height_map("height_map.png");
}

TerrainGenerator::~TerrainGenerator() {

};

void TerrainGenerator::read_height_map(string path) {
	int image_width, image_height, image_n_channels;

	std::filesystem::path file = std::filesystem::current_path() / "..\\Fyredon\\Assets\\Images\\Heightmaps" / path;

	std::cout << file.string().c_str() << "\n";

	unsigned char* data = stbi_load(file.string().c_str(), &image_width, &image_height, &image_n_channels, 0);

	const int x_res = 1081;
	const int z_res = 1081;
	std::vector<GLfloat> vertices;

	for (size_t z = 0; z < z_res; z++){
		for (size_t x = 0; x < x_res ; x++){
		vertices.push_back((GLfloat)(x));
		vertices.push_back((GLfloat)(z));
		vertices.push_back((GLfloat)(data[x*z]));
		}
	}

}