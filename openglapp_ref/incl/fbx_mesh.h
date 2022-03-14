#pragma once
#include "mesh.h"
#include "FBXLoader.h"
class fbxObjectData { // 
public:
	FbxNode* pNode;
	struct
	{
		std::vector<glm::vec3> v_p;
		std::vector<glm::vec3> v_n;
		std::vector<glm::vec2> v_t;
		std::vector<vert> verts;
	} fbx_obj_data;
	struct
	{
		std::vector<glm::vec3> v_p;
		std::vector<glm::vec3> v_n;
		std::vector<glm::vec2> v_t;
		std::vector<vert> verts;
	} fbx_obj_data_backup;
};

class fbx_mesh : public Mesh
{
public:
	fbx_mesh(const char* name, const char* filePath);
	~fbx_mesh();

	void load_obj(bool has_tex = false);
	void reFreshMeshWithCurrentFrame(double time = 0);
	void refreshTransform(fbxObjectData *fbxMeshData,FbxMatrix transformMatrix);
	void obj_data_backup();
	void loadFbxMeshToRender();
public:
	std::vector<fbxObjectData> fbxMeshData;
	FBXLoader* fbxloader;
	bool mHasNormal;
	bool mHasUV;
};

