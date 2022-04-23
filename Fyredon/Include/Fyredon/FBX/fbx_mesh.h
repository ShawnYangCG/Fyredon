// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	FBXMesh.h: Xiaoyuan Yang
=============================================================================*/
#pragma once
#include "Fyredon/Render/Mesh.h"
#include "FBXLoader.h"

class fbxObjectData { // 
public:
	FbxNode* pNode;
	struct mesh_data
	{
		std::vector<glm::vec3> v_p;
		std::vector<glm::vec3> v_n;
		std::vector<glm::vec3> v_t;
		std::vector<Fyredon::Vert> verts;
	};
	mesh_data fbx_obj_data;
	mesh_data fbx_obj_data_backup;
};

class fbx_mesh : public Fyredon::Mesh
{
public:
	fbx_mesh(const char* name, const char* filePath);
	~fbx_mesh();

	void load_obj(bool has_tex = false);
	void reFreshMeshWithCurrentTime(double time = 0);
	void reFreshMeshWithCurrentFrame(double time = 0);
	void playCurrentFrame(int frameIndex = 0);
	void refreshTransform(fbxObjectData *fbxMeshData,FbxMatrix transformMatrix);
	void loadAnimation(double startTime = 0, double endTime = 1.0, double timeStep = 0.02);
	void obj_data_backup();
	void loadFbxMeshToRender();
	void loadFbxMeshToRender(int FrameIndex);
public:
	std::vector<std::vector<fbxObjectData>> fbx_mesh_all_fream;
	std::vector<fbxObjectData> fbxMeshData;
	FBXLoader* fbxloader;
	bool mHasNormal;
	bool mHasUV;
};

