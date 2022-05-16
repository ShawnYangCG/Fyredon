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
#include <sstream>

#pragma warning( disable : 26812) 

// =================== Mesh Implementation ===================

Fyredon::Mesh_OBJ::Mesh_OBJ(const char* name, const char* filePath)
	: Mesh(name), file_path(filePath), has_texcoords(false) {}

// Info : Load Obj, assuming triangle based meshes with indices. 
void Fyredon::Mesh_OBJ::load_obj()
{
	has_texcoords = false; 

	// Check file exists
	std::ifstream in(file_path);
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
		// Faces / Indices
		if (str == "f")
		{
			if (use_tex) // Get each face vertex (w/ texture coords)
			{
				for (int i = 0; i < 3; ++i)
				{
					// Vert Postion & Normal Indices. 
					int32_t i_vp, i_vn, i_vt;
					char c; // scratch write

					// Face Vertex Data
					ss >> i_vp; // v_p index
					ss.get(c); // '/'
					ss.get(c); 
					ss >> i_vt; // 'v_t' index
					ss.get(c); 
					ss.get(c);
					ss >> i_vn; // 'v_n' index

					// Create Vertex
					Vert vertex;
					// neg 1 offset for obj indices '1' based. 
					vertex.pos    = obj_data.v_p[i_vp - 1];
					vertex.normal = obj_data.v_n[i_vn - 1];
					vertex.uv     = obj_data.v_t[i_vt - 1];
					vertex.col    = glm::vec3(1.f, 1.f, 1.f);

					// Append to vert array
					vert_data.push_back(vertex);
				}
			}
			else // Get each face vertex (wo/ texture coords)
			{
				for (int i = 0; i < 3; ++i)
				{
					// Vert Postion & Normal Indices. 
					int32_t i_vp, i_vn;
					char c; // scratch write

					// Face Vertex Data
					ss >> i_vp; // v_p index
					ss.get(); // '/'
					ss.get(); 
					ss >> i_vn; // 'v_n' index

					// Create Vertex
					Vert vertex;
					// neg 1 offset for obj indices '1' based. 
					vertex.pos = obj_data.v_p[i_vp - 1];
					vertex.normal = obj_data.v_n[i_vn - 1];
					vertex.uv = glm::vec3(0.f, 0.f, 0.f);
					vertex.col = glm::vec3(1.f, 1.f, 1.f);

					// Append to vert array
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

