#include "TerrainGenerator.h"
//#define STB_IMAGE_IMPLEMENTATION
#include "../incl/ext/stb/stb_image.h"

TerrainGenerator::TerrainGenerator(const char* name, const char* filePath)
	:Mesh(name, filePath){
	//read_height_map("Heightmap.png");
}

TerrainGenerator::~TerrainGenerator() {

};

void TerrainGenerator::load_obj(bool has_tex) {

}


void TerrainGenerator::read_height_map(std::string path) {
	int image_width, image_height, image_n_channels;

	std::filesystem::path file = std::filesystem::current_path() / "..\\..\\assets\\heightmap" / path;

	std::cout << file.string().c_str() << "\n";

	std::filesystem::path file2 = std::filesystem::current_path() / "..\\..\\assets\\heightmap" / "rewrite.png";

	unsigned char* data = stbi_load(file.string().c_str(), &image_width, &image_height, &image_n_channels, 1);

	int x_res = image_width;
	int z_res = image_height;

	std::cout << x_res << " x_res" << "\n";
	std::cout << z_res << " z_res" << "\n";
	std::cout << image_n_channels << " chan" << "\n";
	std::cout << image_width * image_height * image_n_channels << " image size" << "\n";

	//stbi_write_png(file2.string().c_str(), image_width, image_height, image_n_channels, 1, data, image_width * 3);

	for (size_t z = 0; z < z_res; z++) {
		for (size_t x = 0; x < x_res; x++) {
			glm::vec3 position = glm::vec3(x/10.0,  ((GLfloat)(data[z * z_res + x]) /100.0 ) , z/10.0);
			//std::cout << position.x << " " << " "  << position.z  << " " << position.y << "\n";
			vertices.push_back(position);
		}
	}

	for (size_t z = 0; z < (z_res - 1 ); z++) {
		for (size_t x = 0; x < (x_res - 1 ); x++) {

			glm::vec3 normal = glm::vec3(0, 1, 0);
			float u_text_0 = ((float)x) / x_res;
			float v_text_0 = ((float)z) / z_res;
			float u_text_1 = ((float)x+1) / x_res;
			float v_text_1 = ((float)z+1) / z_res;


			//current
			int i = z * (x_res)+x;
			vert vertex0;
			vertex0.pos = this->vertices[i];
			vertex0.normal = normal;
			vertex0.uv = glm::vec2(u_text_0, v_text_0);
			vertex0.col = glm::vec3(1.f, 1.f, 1.f);

			// right
			vert vertex1;
			vertex1.pos = this->vertices[i + 1];
			vertex1.normal = normal;
			vertex1.uv = glm::vec2(u_text_1, v_text_0);
			vertex1.col = glm::vec3(1.f, 1.f, 1.f);

			// up
			vert vertex2;
			vertex2.pos = this->vertices[i + x_res];
			vertex2.normal = normal;
			vertex2.uv = glm::vec2(u_text_0, v_text_1);
			vertex2.col = glm::vec3(1.f, 1.f, 1.f);

			// up and right
			vert vertex3;
			vertex3.pos = this->vertices[i + x_res + 1];
			vertex3.normal = normal;
			vertex3.uv = glm::vec2(u_text_1, v_text_1);;
			vertex3.col = glm::vec3(1.f, 1.f, 1.f);

			//Set Vector U to(Triangle.p2 minus Triangle.p1)
			//	Set Vector V to(Triangle.p3 minus Triangle.p1)
			//	Set Normal.x to(multiply U.y by V.z) minus(multiply U.z by V.y)
			//	Set Normal.y to(multiply U.z by V.x) minus(multiply U.x by V.z)
			//	Set Normal.z to(multiply U.x by V.y) minus(multiply U.y by V.x)

			glm::vec3 u1 = vertex3.pos - vertex0.pos;
			glm::vec3 v1 = vertex1.pos - vertex0.pos;
			glm::vec3 n1 = glm::normalize(glm::cross(u1, v1));

			glm::vec3 u2 = vertex2.pos - vertex0.pos;
			glm::vec3 v2 = vertex3.pos - vertex0.pos;
			glm::vec3 n2 = glm::normalize(glm::cross(u2, v2));


			vertex0.normal = n1;
			vertex1.normal = n1;
			vertex3.normal = n1;
			obj_data.verts.push_back(vertex0);
			obj_data.verts.push_back(vertex3);
			obj_data.verts.push_back(vertex1);

			vertex0.normal = n2;
			vertex2.normal = n2;
			vertex3.normal = n2;
			obj_data.verts.push_back(vertex0);
			obj_data.verts.push_back(vertex2);
			obj_data.verts.push_back(vertex3);
		}
	}

	//std::cout << obj_data.verts.size() << " size\n";
	//std::cout << "verts " << obj_data.verts.size() << "\n";

	set_data_mesh(obj_data.verts);

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

