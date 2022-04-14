// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	TerrainGeneration.cpp: Terrain loading and management class : Isaiah
=============================================================================*/
#include "Fyredon/Other/pch.h"

#include "Fyredon/Terrain/TerrainGenerator.h"
//#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "Fyredon/Debug/Logger.h"

TerrainGenerator::TerrainGenerator(const char* name, const char* filePath){
	//read_height_map("Heightmap.png");
}

TerrainGenerator::~TerrainGenerator() {

};

void TerrainGenerator::load_obj(bool has_tex) {

}


void TerrainGenerator::read_height_map(std::string path) {
	int image_width, image_height, image_n_channels;

	std::filesystem::path file = std::filesystem::current_path() / "..\\Assets\\Images\\Heightmaps" / path;

	std::cout << file.string().c_str() << "\n";

	std::filesystem::path file2 = std::filesystem::current_path() / "..\\Assets\\heightmap" / "rewrite.png";

	unsigned char* data = stbi_load(file.string().c_str(), &image_width, &image_height, &image_n_channels, 1);

	int x_res = image_width;
	int z_res = image_height;

	std::cout << x_res << " x_res" << "\n";
	std::cout << z_res << " z_res" << "\n";
	std::cout << image_n_channels << " chan" << "\n";
	std::cout << image_width * image_height * image_n_channels << " image size" << "\n";


	for (size_t z = 0; z < z_res; z++) {
		for (size_t x = 0; x < x_res; x++) {
			glm::vec3 position = glm::vec3(x/10.0,  ((float)(data[z * z_res + x]) /100.0 ) , z/10.0);
			//std::cout << position.x << " " << " "  << position.z  << " " << position.y << "\n";
			vertices.push_back(position);
		}
	}
	std::cout << "verts" << " image size" << "\n";


	for (size_t z = 0; z < (z_res - 1 ); z++) {
		for (size_t x = 0; x < (x_res - 1 ); x++) {

			float u_text_0 = ((float)x) / x_res;
			float v_text_0 = ((float)z) / z_res;
			float u_text_1 = ((float)x+1) / x_res;
			float v_text_1 = ((float)z+1) / z_res;


			//current
			int i = z * (x_res)+x;
			Fyredon::vert vertex0;
			vertex0.pos = this->vertices[i];
			vertex0.uv = glm::vec2(u_text_0, v_text_0);
			vertex0.col = glm::vec3(1.f, 1.f, 1.f);

			// right
			Fyredon::vert vertex1;
			vertex1.pos = this->vertices[i + 1];
			vertex1.uv = glm::vec2(u_text_1, v_text_0);
			vertex1.col = glm::vec3(1.f, 1.f, 1.f);

			// up
			Fyredon::vert vertex2;
			vertex2.pos = this->vertices[i + x_res];
			vertex2.uv = glm::vec2(u_text_0, v_text_1);
			vertex2.col = glm::vec3(1.f, 1.f, 1.f);

			// up and right
			Fyredon::vert vertex3;
			vertex3.pos = this->vertices[i + x_res + 1];
			vertex3.uv = glm::vec2(u_text_1, v_text_1);;
			vertex3.col = glm::vec3(1.f, 1.f, 1.f);

			//Set Vector U to(Triangle.p2 minus Triangle.p1)
			//	Set Vector V to(Triangle.p3 minus Triangle.p1)
			//	Set Normal.x to(multiply U.y by V.z) minus(multiply U.z by V.y)
			//	Set Normal.y to(multiply U.z by V.x) minus(multiply U.x by V.z)
			//	Set Normal.z to(multiply U.x by V.y) minus(multiply U.y by V.x)

			glm::vec3 u1 = vertex1.pos - vertex0.pos;
			glm::vec3 v1 = vertex3.pos - vertex0.pos;
			glm::vec3 n1 = glm::normalize(glm::cross(u1, v1));

			glm::vec3 u2 = vertex3.pos - vertex0.pos;
			glm::vec3 v2 = vertex2.pos - vertex0.pos;
			glm::vec3 n2 = glm::normalize(glm::cross(u2, v2));


			vertex0.normal = n1;
			vertex1.normal = n1;
			vertex3.normal = n1;
			TerrainMesh.obj_data.verts.push_back(vertex0);
			TerrainMesh.obj_data.verts.push_back(vertex3);
			TerrainMesh.obj_data.verts.push_back(vertex1);

			vertex0.normal = n2;
			vertex2.normal = n2;
			vertex3.normal = n2;
			TerrainMesh.obj_data.verts.push_back(vertex0);
			TerrainMesh.obj_data.verts.push_back(vertex2);
			TerrainMesh.obj_data.verts.push_back(vertex3);
			//std::cout << "n1 " << n1.x << " " << n1.y << " " << n1.z << "\n";
			//std::cout << "n2 " << n2.x << " " << n2.y << " " << n2.z << "\n";
		}
	}

	std::cout << TerrainMesh.obj_data.verts.size() << " size\n";
	std::cout << "verts " << TerrainMesh.obj_data.verts.size() << "\n";

	TerrainMesh.set_data_mesh(TerrainMesh.obj_data.verts);

}


float TerrainGenerator::heightFromIndex(int index) {
	if (index < this->vertices.size()) {
		return this->vertices[index].y;
	}	else {
		return NULL;
	}

}

//TODO return height from given (x,z)

//TODO apply textures

//TODO move verts based on (x,z) and radius

