// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	Mesh.h: Wavefront .obj based mesh : Niall
=============================================================================*/
#pragma once

// Std Headers
#include <string>
#include <vector>

// Ext Headers 
// GLM
#include "glm/glm.hpp"

// Project Headers
#include "primitive.h"
#include "texture.h"

/* Info : Primitive derived class for rendering meshes based on loaded.obj file.
* Vertex Attribute layout same as Primitive (P(x,y,z), N(x,y,z), C(r,g,b), UV(u,v,w))
* This does not use indexing. 
* Only use this class for OBJ based mesh files, for manual vertex/index specifcation, use Primitive. */

namespace Fyredon
{
class Mesh : public Primitive
{
public:
	Mesh() : Primitive("", false) {}
	Mesh(const char *name, const char *filePath);
	virtual ~Mesh() = default; 

	void load_obj();

private:
	std::string file_path; 
	// ToDo: Consider making this data local storage only on loading. 
	struct
	{
		std::vector<glm::vec3> v_p;
		std::vector<glm::vec3> v_n;
		std::vector<glm::vec3> v_t; 
	} obj_data;

	bool has_texcoords;
};
}


