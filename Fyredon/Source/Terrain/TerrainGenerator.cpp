// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	TerrainGeneration.cpp: Terrain loading and management class : Isaiah
=============================================================================*/
#include "Fyredon/Other/pch.h"

#include "Fyredon/Terrain/TerrainGenerator.h"
//#define STB_IMAGE_IMPLEMENTATION


TerrainGenerator::TerrainGenerator(const char* name, const char* filePath){
	//read_height_map("Heightmap.png");
}

TerrainGenerator::~TerrainGenerator() {

};

void TerrainGenerator::load_obj(bool has_tex) {

}


void TerrainGenerator::write_obj(std::string path) {

}


void TerrainGenerator::read_height_map(std::string path) {

	std::filesystem::path file = std::filesystem::current_path() / "..\\Assets\\Images\\Heightmaps" / path;

	//std::cout << file.string().c_str() << "\n";

	std::filesystem::path file2 = std::filesystem::current_path() / "..\\Assets\\heightmap" / "rewrite.png";

	image_data = stbi_load(file.string().c_str(), &image_width, &image_height, &image_n_channels, 1);

	int x_res = image_width;
	int z_res = image_height;

	//std::cout << x_res << " x_res" << "\n";
	//std::cout << z_res << " z_res" << "\n";
	//std::cout << image_n_channels << " chan" << "\n";
	//std::cout << image_width * image_height * image_n_channels << " image size" << "\n";
}


float TerrainGenerator::bilinear_interpolation(float x, float z) {
	//Outside of terrain
	if (x > image_width || z > image_height || x < 0 || z < 0)
		return NULL;

	float x_whole, x_fraction, z_whole, z_fraction;
	x_fraction = std::modf(x, &x_whole);
	z_fraction = std::modf(z, &z_whole);

	int i = z_whole * (image_width)+x_whole;

	// on center. return pixel value
	if (x_fraction == 0 && z_fraction == 0) {
		return  image_data[i];
	}

	//left bot
	glm::vec3 vertex0 = glm::vec3(x_whole, image_data[i], z_whole);

	// right
	glm::vec3 vertex1 = glm::vec3(x_whole + 1, image_data[i+ 1 ], z_whole);

	// left up
	glm::vec3 vertex2 = glm::vec3(x_whole, image_data[i + image_width], z_whole + 1);

	// up and right
	glm::vec3 vertex3 = glm::vec3(x_whole+ 1 , image_data[i + image_width + 1], z_whole + 1);

	if (x_fraction >= z_fraction) {
		vertex2 = vertex0 + (vertex1 - vertex3);
	}
	else {
		vertex1 = vertex0 + (vertex3 - vertex2);
	}


	glm::vec3 v01 = glm::vec3(x, (vertex0.y * x_fraction) + (vertex1.y * (1 - x_fraction)), z_whole);
	glm::vec3 v23 = glm::vec3(x, (vertex2.y * x_fraction) + (vertex3.y * (1 - x_fraction)), z_whole + 1);
	glm::vec3 v0123 = glm::vec3(x, (v01.y *z_fraction) + (v23.y * (1 - z_fraction)), z);

	return (v01.y * z_fraction) + (v0123.y * (1 - z_fraction));
}

float TerrainGenerator::triangle_plane(float x, float z){
	//Outside of terrain
	if (x > image_width || z > image_height || x < 0 || z < 0)
		return NULL;

	float x_whole, x_fraction, z_whole, z_fraction;
	x_fraction = std::modf(x, &x_whole);
	z_fraction = std::modf(z, &z_whole);

	int i = z_whole * (image_width)+x_whole;

	// on center. return pixel value
	if (x_fraction == 0 && z_fraction == 0) {
	
		return  image_data[i];
	}

	if (x_whole == image_width || z_whole == image_height) {
		return  image_data[i];
	}


	//left bot
	glm::vec3 A = glm::vec3(x_whole, image_data[i], z_whole);

	// right
	glm::vec3 B = glm::vec3(x_whole + 1, image_data[i + 1], z_whole);

	// left up
	glm::vec3 B2 = glm::vec3(x_whole, image_data[i + image_width], z_whole + 1);

	// up and right
	glm::vec3 C = glm::vec3(x_whole + 1, image_data[i + image_width + 1], z_whole + 1);

	glm::vec3 normal;

	if (x_fraction >= z_fraction) {
		normal = glm::cross((C - A), (B - A));

		return  B.y + (1 - x_fraction) * (A - B).y + (z_fraction) * (C - B).y;

	}
	else {
		return  B2.y + (x_fraction) * (C - B2).y + (1-z_fraction) * (A - B2).y;

	}


	//glm::vec3 v01 = glm::vec3(x, (vertex0.y * x_fraction) + (vertex1.y * (1 - x_fraction)), z_whole);
	//glm::vec3 v23 = glm::vec3(x, (vertex2.y * x_fraction) + (vertex3.y * (1 - x_fraction)), z_whole + 1);
	//glm::vec3 v0123 = glm::vec3(x, (v01.y * z_fraction) + (v23.y * (1 - z_fraction)), z);



	return 0;// (v01.y * z_fraction) + (v0123.y * (1 - z_fraction));
}



void TerrainGenerator::generate_mesh(int res) {
	
	// Allocate Primitive members 
	TerrainMesh = new Fyredon::Mesh("Terrain_Mesh"); 

	// Temp storage
	std::vector<Fyredon::Vert> verts; 
	std::vector<Fyredon::GLuint> inds;
	
	//std::cout << "z res " << image_height/((float)image_height / (float)res) << "\n";
	//std::cout << "x res " << image_width/((float)image_width / (float)res) << "\n";

	terrain_res = res;

	for (float z = 0; z < image_height; z += ((float) image_height / (float) res)) {
		for (float x = 0; x < image_width; x += ((float) image_width / (float) res)) {
			glm::vec3 position = glm::vec3(x/10.0, (triangle_plane(x,z) /400.0 ) , z/10.0);
			vertices.push_back(position);
		}
	}
	std::cout << vertices.size() << " vert size\n";

	//smooth_normals();

	//std::cout << "verts" << " image size" << "\n";

	for (size_t z = 0; z < (res - ((float)res / (float)image_height)); z++) {
		for (size_t x = 0; x < (res - ((float)res / (float)image_width)); x++) {
			//UVs
			float u_text_0 = ((float)x) / res;
			float v_text_0 = ((float)z) / res;
			float u_text_1 = ((float)x+1) / res;
			float v_text_1 = ((float)z+1) / res;

			// Verts
			//current
			int i = (z * res) + x;
			Fyredon::Vert vertex0;
			vertex0.pos = this->vertices[i];
			vertex0.uv = glm::vec3(0.f, 0.f, 0.f);
			vertex0.col = glm::vec3(1.f, 1.f, 1.f);

			// right
			Fyredon::Vert vertex1;
			vertex1.pos = this->vertices[i + 1];
			vertex1.uv = glm::vec3(0.f, 0.f, 0.f);
			vertex1.col = glm::vec3(1.f, 1.f, 1.f);

			// up
			Fyredon::Vert vertex2;
			vertex2.pos = this->vertices[i + res];
			vertex2.uv = glm::vec3(0.f, 0.f, 0.f);
			vertex2.col = glm::vec3(1.f, 1.f, 1.f);

			// up and right
			Fyredon::Vert vertex3;
			vertex3.pos = this->vertices[i + res + 1];
			vertex3.uv = glm::vec3(0.f, 0.f, 0.f);
			vertex3.col = glm::vec3(1.f, 1.f, 1.f);

			
			Fyredon::vert vertex4;
			Fyredon::vert vertex5;
			Fyredon::vert vertex6;
			Fyredon::vert vertex7;
			Fyredon::vert vertex8;
			Fyredon::vert vertex9;


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
			verts.push_back(vertex0);
			verts.push_back(vertex3);
			verts.push_back(vertex1);

			vertex0.normal = n2;
			vertex2.normal = n2;
			vertex3.normal = n2;
			verts.push_back(vertex0);
			verts.push_back(vertex2);
			verts.push_back(vertex3);
			//std::cout << "n1 " << n1.x << " " << n1.y << " " << n1.z << "\n";
			//std::cout << "n2 " << n2.x << " " << n2.y << " " << n2.z << "\n";
		}
	}

	//std::cout << TerrainMesh.obj_data.verts.size() << " size\n";
	//std::cout << "verts " << vertices.size() << "\n";

	std::filesystem::path file = std::filesystem::current_path() / "..\\Assets\\Images\\Terrain\\grass.jpg";
	std::string file_as_string = file.string();
	

	//grass = Fyredon::TextureImage(file_as_string.c_str(), "grass", 1);
	TerrainMesh.load_texture(file_as_string.c_str(),0);
	
	TerrainMesh.set_data_mesh(TerrainMesh.obj_data.verts);
}


void TerrainGenerator::smooth_normals() {
	normals.clear();

	// assume terrain is square
	int sqrt_res = sqrt(terrain_res);

	/// UL	U	UR
	/// L	M	R
	/// LL	L	LR
	int LL = 0;
	int LR = terrain_res ;
	int UR = terrain_res * terrain_res - 1; 
	int UL = UR - terrain_res;

	std::cout << terrain_res << " terrain res \n";

	for (size_t i = 0; i < vertices.size(); i++){
		//std::cout << i << " ";
		//lower left
		if (i == 0) {
			//std::cout << "LL\n";
			glm::vec3 A = vertices[i];
			glm::vec3 B = vertices[i + terrain_res];
			glm::vec3 C = vertices[i + terrain_res + 1];
			glm::vec3 D = vertices[i + 1];

			glm::vec3 n = glm::normalize((B - A) + (C - A) + (D - A));
			normals.push_back(n);
		}
		//lower right
		else if (i == LR) {
			//std::cout << "LR\n";

			glm::vec3 A = vertices[i];
			glm::vec3 B = vertices[i + terrain_res];
			glm::vec3 C = vertices[i - 1];

			glm::vec3 n = glm::normalize((B - A) + (C - A) );
			normals.push_back(n);
		}
		// upper left
		else if (i == UL) {
			//std::cout << "UL\n";

			glm::vec3 A = vertices[i];
			glm::vec3 B = vertices[i + 1];
			glm::vec3 C = vertices[i - terrain_res];
		
			glm::vec3 n = glm::normalize((B - A) + (C - A));
			normals.push_back(n);
		}
		// upper right
		else if (i == UR) {
			//std::cout << "UR\n";

			glm::vec3 A = vertices[i];
			glm::vec3 B = vertices[i - terrain_res];
			glm::vec3 C = vertices[i - terrain_res - 1];
			glm::vec3 D = vertices[i - 1];

			glm::vec3 n = glm::normalize((B - A) + (C - A) + (D - A));
			normals.push_back(n);
		}
		//lower
		else if (i > LL && i < LR) {
			//std::cout << "L\n";

			glm::vec3 A = vertices[i];
			glm::vec3 B = vertices[i + terrain_res];
			glm::vec3 C = vertices[i + terrain_res + 1];
			glm::vec3 D = vertices[i + 1];
			glm::vec3 E = vertices[i - 1];

			glm::vec3 n = glm::normalize((B - A) + (C - A) + (D - A) + (E - A));
			normals.push_back(n);
		}
		//left
		else if (i % terrain_res == 0 && i > LL && i < UL) {
			//std::cout << "L\n";

			
			glm::vec3 A = vertices[i];
			glm::vec3 B = vertices[i + terrain_res];
			glm::vec3 C = vertices[i + terrain_res + 1];
			glm::vec3 D = vertices[i + 1];
			glm::vec3 E = vertices[i - terrain_res];

			glm::vec3 n = glm::normalize((B - A) + (C - A) + (D - A) + (E - A));
			normals.push_back(n);
		}
		//right
		else if (i % terrain_res == terrain_res - 1 && i > LR && i != UR  ) {
			//std::cout << "R\n";

			glm::vec3 A = vertices[i];
			glm::vec3 B = vertices[i + terrain_res];
			glm::vec3 C = vertices[i - terrain_res];
			glm::vec3 D = vertices[i - terrain_res - 1];
			glm::vec3 E = vertices[i - 1];

			glm::vec3 n = glm::normalize((B - A) + (C - A) + (D - A));
			normals.push_back(n);
		}
		// upper
		else if (i > UL && i < UR) {
			//std::cout << "U\n";

			glm::vec3 A = vertices[i];
			glm::vec3 B = vertices[i + 1];
			glm::vec3 C = vertices[i - terrain_res];
			glm::vec3 D = vertices[i - terrain_res - 1];
			glm::vec3 E = vertices[i - 1];

			glm::vec3 n = glm::normalize((B - A) + (C - A) + (D - A));
			normals.push_back(n);
		}
		//mids
		else {
			//std::cout << "M\n";

			glm::vec3 A = vertices[i];
			glm::vec3 B = vertices[i + terrain_res];
			glm::vec3 C = vertices[i + terrain_res + 1];
			glm::vec3 D = vertices[i + 1];
			glm::vec3 E = vertices[i - terrain_res];
			glm::vec3 F = vertices[i - terrain_res - 1];
			glm::vec3 G = vertices[i - 1];

			glm::vec3 n = glm::normalize((B - A) + (C - A) + (D - A) + (E - A) + (F - A) + (G - A));
			normals.push_back(n);
		}
	}
	std::cout << vertices.size() << " verts size \n";
	std::cout << normals.size() << " normals size \n";
}



float TerrainGenerator::heightFromIndex(int index) {
	if (index < this->vertices.size()) {
		return this->vertices[index].y;
	}
	return NULL;
}

float TerrainGenerator::heightFromXZ(int x, int z) {
	//TODO non int heights
	if (x < image_width && z < image_height ) {
		int i = z * (image_width) + x;
		return this->vertices[i].y;
	}
	return NULL;
}

void TerrainGenerator::draw_water_quad() {

	water_quad  = new Fyredon::Mesh("Water Quad");
	std::vector<Fyredon::Vert> verts; 

	int res = 513;

	for (size_t z = 0; z < res; z += res-1) {
		for (size_t x = 0; x < res; x += res-1) {

			glm::vec3 position = glm::vec3(x / 10.0, 10, z / 10.0);
			//std::cout << position.x << " " << " "  << position.z  << " " << position.y << "\n";

			Fyredon::Vert vertex;
			vertex.pos = position;
			vertex.uv = glm::vec3((x/(res - 1)), (z/(res - 1)), 0.f);
			vertex.col = glm::vec3(0, 0, 1.f);
			vertex.normal = glm::vec3(0, 1.f, 0);

			verts.push_back(vertex);

		}
	}

	// Set Primtivie Data
	water_quad->set_data_mesh(verts);
}

//TODO return height from given (x,z)

//TODO apply textures
void TerrainGenerator::load_terrain_image(std::string path) {
	std::filesystem::path file = std::filesystem::current_path() / "..\\Assets\\Images\\grass.jpg" / path;

	const char* file_as_string = file.string().c_str();

	//grass = Fyredon::TextureImage(file_as_string, "grass", 1);

	//grass.load();
	//std::cout << grass.valid_state << " valid state\n";

}

//TODO move verts based on (x,z) and radius

