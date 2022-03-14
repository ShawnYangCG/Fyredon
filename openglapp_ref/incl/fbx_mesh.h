#pragma once
#include "mesh.h"
#include "FBXLoader.h"
class fbx_mesh : public Mesh
{
public:
	fbx_mesh(const char* name, const char* filePath);
	~fbx_mesh();

	void load_obj(bool has_tex = false);
	void reFreshMeshWithCurrentFrame(double time = 0);
	void refreshTransform(FbxMatrix transformMatrix);
	void obj_data_backup();
public:
	struct
	{
		std::vector<glm::vec3> v_p;
		std::vector<glm::vec3> v_n;
		std::vector<glm::vec2> v_t;
		std::vector<vert> verts;
	} obj_data_ori;

	FBXLoader* fbxloader;
	bool mHasNormal;
	bool mHasUV;
};

