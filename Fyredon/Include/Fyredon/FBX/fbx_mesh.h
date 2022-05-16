// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	FBXMesh.h: Xiaoyuan Yang
=============================================================================*/
#pragma once
#include "Fyredon/Render/Mesh_OBJ.h"
#include "FBXLoader.h"

class fbxDataInOneFrame { // 
public:
	FbxNode* pNode;
	std::vector<Fyredon::Vert> verts;
	/*
	struct mesh_data
	{
		std::vector<glm::vec3> v_p;
		std::vector<glm::vec3> v_n;
		std::vector<glm::vec3> v_t;
		std::vector<Fyredon::Vert> verts;
	};
	mesh_data fbx_obj_data;
	mesh_data fbx_obj_data_backup;*/
};

class fbx_mesh : public Fyredon::Mesh
{
public:
	fbx_mesh(const char* name, const char* filePath);
	~fbx_mesh();
	void load_obj(bool has_tex = false);
	std::vector<fbxDataInOneFrame> reFreshMeshWithCurrentTime(double time = 0);
	void reFreshMeshWithCurrentFrame(double time = 0);
	void playCurrentFrame(int frameIndex = 0);
	fbxDataInOneFrame refreshTransform(fbxDataInOneFrame* fbxMeshData, FbxMatrix transformMatrix);
	void loadAnimation(double startTime = 0, double endTime = 1.0, double timeStep = 0.02);
	void obj_data_backup();
	void loadFbxMeshToRender();
	void loadFbxMeshToRender(int FrameIndex);
private:
	std::string file_path;

public:
	std::vector<std::vector<fbxDataInOneFrame>> fbx_mesh_all_fream;
	std::vector<fbxDataInOneFrame> fbxMeshData;
	//std::vector<fbxDataInOneFrame>* currentFrame;
	FBXLoader* fbxloader;
};

