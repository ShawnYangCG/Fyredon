#include "TerrainGenerator.h"
//#define STB_IMAGE_IMPLEMENTATION
#include "incl/ext/stb/stb_image.h"

TerrainGenerator::TerrainGenerator(const char* name, const char* filePath)
	:Mesh(name, filePath){
	read_height_map("height_map.png");
}

TerrainGenerator::~TerrainGenerator() {

};

void TerrainGenerator::load_obj(bool has_tex) {

}


void TerrainGenerator::read_height_map(std::string path) {
	int image_width, image_height, image_n_channels;

	std::filesystem::path file = std::filesystem::current_path() / "..\\..\\assets\\heightmap" / path;

	std::cout << file.string().c_str() << "\n";

	unsigned char* data = stbi_load(file.string().c_str(), &image_width, &image_height, &image_n_channels, 0);

	const int x_res = 1081;
	const int z_res = 1081;
	

	for (size_t z = 0; z < z_res; z++) {
		for (size_t x = 0; x < x_res; x++) {
			glm::vec3 position = glm::vec3(x,  ((GLfloat)(data[x * z])/10.0) , z);
			//std::cout << x << " " << " "  << z  << " " << (GLfloat)(data[x * z]) << "\n";
			vertices.push_back(position);
		}
	}

	for (size_t i = 0; i < (x_res - 1) * (z_res - 1) ; i++)
	{
		//this->obj_data.v_p.emplace_back(this->vertices[i]);
		glm::vec3 normal = glm::vec3(0, 1, 0);
		//this->obj_data.v_n.emplace_back(normal);
		glm::vec2 uv = glm::vec2(0,0);
		//this->obj_data.v_t.emplace_back(uv);

		vert vertex0;
		vertex0.pos = this->vertices[i];
		vertex0.normal = normal;
		vertex0.uv = uv;
		vertex0.col = glm::vec3(1.f, 1.f, 1.f);

		vert vertex1;
		vertex1.pos = this->vertices[i+1];
		vertex1.normal = normal;
		vertex1.uv = uv;
		vertex1.col = glm::vec3(1.f, 1.f, 1.f);

		vert vertex2;
		vertex2.pos = this->vertices[i+x_res];
		vertex2.normal = normal;
		vertex2.uv = uv;
		vertex2.col = glm::vec3(1.f, 1.f, 1.f);

		vert vertex3;
		vertex3.pos = this->vertices[i+x_res+1];
		vertex3.normal = normal;
		vertex3.uv = uv;
		vertex3.col = glm::vec3(1.f, 1.f, 1.f);

		//Set Vector U to(Triangle.p2 minus Triangle.p1)
		//	Set Vector V to(Triangle.p3 minus Triangle.p1)

		//	Set Normal.x to(multiply U.y by V.z) minus(multiply U.z by V.y)
		//	Set Normal.y to(multiply U.z by V.x) minus(multiply U.x by V.z)
		//	Set Normal.z to(multiply U.x by V.y) minus(multiply U.y by V.x)

		glm::vec3 u1 = vertex3.pos - vertex0.pos;
		glm::vec3 v1 = vertex1.pos - vertex0.pos;
		glm::vec3 n1 = glm::cross(u1, v1);

		glm::vec3 u2 = vertex2.pos - vertex0.pos;
		glm::vec3 v2 = vertex3.pos - vertex0.pos;
		glm::vec3 n2 = glm::cross(u2, v2);


		
		vertex0.normal = n1;
		vertex1.normal = n1;
			vertex2.normal = n2;
			vertex3.normal = n2;

		obj_data.verts.push_back(vertex0);
		obj_data.verts.push_back(vertex3);
		obj_data.verts.push_back(vertex1);
		obj_data.verts.push_back(vertex0);
		obj_data.verts.push_back(vertex2);
		obj_data.verts.push_back(vertex3);
	}



	std::cout << "verts " << obj_data.verts.size() << "\n";

	set_data_mesh(obj_data.verts);

}
