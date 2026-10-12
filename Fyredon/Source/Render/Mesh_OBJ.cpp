// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	Mesh.cpp: .obj based mesh : Niall
=============================================================================*/
#include "Fyredon/Other/pch.h"
// Implements
#include "Fyredon/Render/Mesh_OBJ.h"

// Extern Headers
#include "GLEW/glew.h"

// Std Headers
#include <fstream>
#include <filesystem>
#include <sstream>

#pragma warning( disable : 26812) 

// =================== Mesh Implementation ===================

Fyredon::Mesh_OBJ::Mesh_OBJ(const char* name, const char* filePath)
	: Mesh(name), file_path(filePath), has_texcoords(false) {}

// Info : Load indexed OBJ triangles, quads and convex polygon faces. 
void Fyredon::Mesh_OBJ::load_obj()
{
	has_texcoords = false;
	obj_data.v_p.clear();
	obj_data.v_n.clear();
	obj_data.v_t.clear();
	vert_data.clear();

	// Check file exists
	std::ifstream in(std::filesystem::u8path(file_path));
	if (!in.is_open())
	{
		std::cerr << "ERROR::Mesh_OBJ::" << name << ":: Invalid .obj file passed" << std::endl;
		return;
	}

	std::ostringstream dbg; 
	std::string line;
	while (std::getline(in, line))
	{
		// Extract first block as str. 
		std::string str;
		std::istringstream ss(line);
		ss >> str;

		dbg << "DEBUG " << str << "\n";

		if (str == "#" || str == "g" || str == "s") continue;

		// Vertex Postion
		if (str == "v")
		{
			float xx, yy, zz;
			ss >> xx;
			ss >> yy;
			ss >> zz;
			obj_data.v_p.emplace_back(xx, yy, zz);
			dbg << "v_" << obj_data.v_p.size() << " = " << xx << "," << yy << "," << zz << "\n";
		}
		// Vertex Normal 
		if (str == "vn")
		{
			float xx, yy, zz;
			ss >> xx;
			ss >> yy;
			ss >> zz;
			obj_data.v_n.emplace_back(xx, yy, zz);
			dbg << "vn_" << obj_data.v_n.size() << " = " << xx << "," << yy << "," << zz << "\n";
		}
		// Vertex Texture
		if (str == "vt")
		{
			// Set texture state if 'vt' found. 
			has_texcoords |= true; 
			use_tex |= true; 

			float uu, vv;
			ss >> uu;
			ss >> vv;
			obj_data.v_t.emplace_back(uu, vv, 0.f);
			dbg << "vt_" << obj_data.v_t.size() << " = " << uu << "," << vv << "\n";
		}
		// Read the complete face, then triangulate it without changing its winding.
		if (str == "f")
		{
			std::vector<Vert> face;
			std::string token;
			while (ss >> token)
			{
				if (token[0] == '#') break;
				std::vector<std::string> indices;
				std::istringstream fields(token);
				std::string field;
				while (std::getline(fields, field, '/')) indices.push_back(field);
				auto index = [&](const std::string& value, std::size_t count) {
					const int raw = std::stoi(value);
					const int resolved = raw > 0 ? raw - 1 : static_cast<int>(count) + raw;
					if (raw == 0 || resolved < 0 || resolved >= static_cast<int>(count))
						throw std::runtime_error("Invalid OBJ index in " + file_path);
					return resolved;
				};
				Vert vertex;
				vertex.pos = obj_data.v_p.at(index(indices.at(0), obj_data.v_p.size()));
				vertex.normal = glm::vec3(0.f);
				vertex.uv = glm::vec3(0.f);
				vertex.col = glm::vec3(1.f);
				// Support standard v/vt/vn and v//vn, plus this project's v//vt//vn.
				const bool legacy = indices.size() == 5;
				const std::size_t uvSlot = legacy ? 2 : 1;
				const std::size_t normalSlot = legacy ? 4 : 2;
				if (indices.size() > uvSlot && !indices[uvSlot].empty())
					vertex.uv = obj_data.v_t.at(index(indices[uvSlot], obj_data.v_t.size()));
				if (indices.size() > normalSlot && !indices[normalSlot].empty())
					vertex.normal = obj_data.v_n.at(index(indices[normalSlot], obj_data.v_n.size()));
				face.push_back(vertex);
			}
			if (face.size() < 3) throw std::runtime_error("Incomplete OBJ face in " + file_path);
			// Triangle fan supports triangles, quads and convex polygons.
			for (std::size_t i = 1; i + 1 < face.size(); ++i)
			{
				Vert triangle[] = { face[0], face[i], face[i + 1] };
				const glm::vec3 crossNormal = glm::cross(triangle[1].pos - triangle[0].pos,
					triangle[2].pos - triangle[0].pos);
				for (auto& vertex : triangle) {
					if (glm::length(vertex.normal) == 0.f && glm::length(crossNormal) > 0.f)
						vertex.normal = glm::normalize(crossNormal);
					vert_data.push_back(vertex);
				}
			}
		}
	} // End file read loop. 
	// Close 
	in.close();

	dbg << "Vert Count = " << vert_data.size() << "\n";

	// Pass Mesh Data for indexing and buffer creation
	set_data_mesh(vert_data);

	// Check Primitive is correct.
	//debug();

#ifdef DEBUG_LOG
		std::cout << dbg.str();
#endif
}

