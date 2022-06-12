#include "Fyredon/Other/pch.h"
#include "Fyredon/FBX/fbx_mesh.h"

fbx_mesh::fbx_mesh(const char* name, const char* filePath)
	:Mesh(name)
{
	this->fbxloader = new FBXLoader(filePath);
	this->fbxloader->LoadFile();
}

fbx_mesh::~fbx_mesh()
{
	if (this->fbxloader) free(this->fbxloader);
}

void fbx_mesh::load_obj(bool has_tex)
{
	// print all node in fbx
	FbxScene* lScene = this->fbxloader->Scene;
	FbxNode* lRootNode = lScene->GetRootNode();
	if (lRootNode) {
		//std::cout << "Child node count: " << lRootNode->GetChildCount() << std::endl;
		//for (int i = 0; i < lRootNode->GetChildCount(); i++)
			//this->fbxloader->PrintNode(lRootNode->GetChild(i));
	}

	// get fbxmesh from one node
	if (lRootNode) {
		for (int i = 0; i < lRootNode->GetChildCount(); i++) {
			FbxNode* pNode = lRootNode->GetChild(i);
			for (int index = 0; index < pNode->GetNodeAttributeCount(); index++) {
				FbxString typeName = 
					this->fbxloader->GetAttributeTypeName(
						pNode->GetNodeAttributeByIndex(index)->GetAttributeType());
				if (typeName == "mesh") {
					fbxDataInOneFrame fbxData = fbxDataInOneFrame();
					fbxData.pNode = pNode;
					FbxMesh *nMesh = pNode->GetMesh();
					VBOMesh vboMesh = VBOMesh();
					vboMesh.Initialize(nMesh);
					/*
					std::cout << "Name: " << pNode->GetName();
					std::cout << " Vertices count: " << vboMesh.lVerticesLength;
					std::cout << " Normals count: " << vboMesh.lNormalsLength;
					std::cout << " UV count: " << vboMesh.lUVsLength << std::endl;*/
					for (int position = 0, normal = 0, texture = 0;
						position < vboMesh.lVerticesLength;
						position += 4, normal += 3, texture += 2) 
					{
						//std::cout << position << "," << normal << "," << texture << "," << std::endl;
						Fyredon::Vert vertex;
						vertex.pos[0] = vboMesh.lVertices[position];
						vertex.pos[1] = vboMesh.lVertices[position + 1];
						vertex.pos[2] = vboMesh.lVertices[position + 2];
						vertex.normal[0] = vboMesh.lNormals[normal];
						vertex.normal[1] = vboMesh.lNormals[normal + 1];
						vertex.normal[2] = vboMesh.lNormals[normal + 2];
						vertex.uv[0] = vboMesh.lUVs[texture];
						vertex.uv[1] = vboMesh.lUVs[texture + 1];
						vertex.uv[2] = 0;
						vertex.col = glm::vec3(1, 1, 1);
						fbxData.verts.push_back(vertex);
						this->vert_data.push_back(vertex);
					}
					this->fbxMeshData.push_back(fbxData);
				}
			}		
		}
	}
	loadAnimation(0.0, 1.0, 0.02);
	loadFbxMeshToRender(0);
	set_data_mesh(this->vert_data);
}

std::vector<fbxDataInOneFrame> fbx_mesh::reFreshMeshWithCurrentTime(double time)
{
	std::vector<fbxDataInOneFrame> allMeshInOneFrame;
	for (int i = 0; i < this->fbxMeshData.size(); i++) {
		FbxScene* lScene = this->fbxloader->Scene;
		FbxAnimEvaluator* mySceneEvaluator = this->fbxloader->Scene->GetAnimationEvaluator();
		FbxTime myTime;       // The time for each key in the animation curve(s)
		myTime.SetSecondDouble(time);   // Starting time

		// Get a reference to node¡¯s global transform.
		//FbxMatrix GlobalMatrix = mySceneEvaluator->GetNodeGlobalTransform(myMeshNode, myTime);
		// Get a reference to node¡¯s local transform.
		FbxNode* myMeshNode = this->fbxMeshData[i].pNode;
		FbxMatrix LocalMatrix = mySceneEvaluator->GetNodeLocalTransform(myMeshNode, myTime);

		allMeshInOneFrame.push_back(refreshTransform(&this->fbxMeshData[i], LocalMatrix));
	}
	return allMeshInOneFrame;
}

void fbx_mesh::reFreshMeshWithCurrentFrame(double time)
{
	
	reFreshMeshWithCurrentTime(time);
	//set_data_mesh(obj_data.verts);
	loadFbxMeshToRender();
}

void fbx_mesh::playCurrentFrame(int frameIndex)
{
	loadFbxMeshToRender(frameIndex);

	std::vector<Fyredon::Vert> unqiue_verts;
	std::vector<GLuint> inds;

	std::unordered_map<Fyredon::Vert, std::size_t, Fyredon::VertHash> vert_map;

	for (std::size_t v = 0; v < this->vert_data.size(); ++v)
	{
		const Fyredon::Vert& vert_i = this->vert_data[v];

		// Does vertex exist in map ? 
		auto found = vert_map.find(vert_i);

		if (found == vert_map.end()) // Not in map, add uniq_vert
		{
			unqiue_verts.push_back(vert_i);
			// Get idx and store as value
			std::size_t idx = unqiue_verts.size() - 1;
			vert_map[vert_i] = idx;
			inds.push_back(idx);
		}
		else // In map, get ind
		{
			inds.push_back(found->second);
		}
	}
	flags.inds_data_set = 1;

	// Copy Vertex Data and Indices Data
	vert_data = unqiue_verts;
	vert_count = unqiue_verts.size();
	inds_data = inds;
	index_count = inds.size();
	this->update_data_mesh(this->vert_data);
}

fbxDataInOneFrame fbx_mesh::refreshTransform(fbxDataInOneFrame* fbxMeshData, FbxMatrix transformMatrix)
{
	fbxDataInOneFrame oneFrame;
	oneFrame.pNode = fbxMeshData->pNode;
	for (int index = 0; index < fbxMeshData->verts.size(); index++) {
		// refresh pos
		double x = fbxMeshData->verts[index].pos[0];
		double y = fbxMeshData->verts[index].pos[1];
		double z = fbxMeshData->verts[index].pos[2];
		FbxVector4 pos = FbxVector4(x, y, z);
		FbxVector4 newPos = transformMatrix.MultNormalize(pos);
		Fyredon::Vert vertex;
		vertex.pos[0] = newPos.mData[0];
		vertex.pos[1] = newPos.mData[1];
		vertex.pos[2] = newPos.mData[2];
		
		// refresh normal
		x = fbxMeshData->verts[index].normal[0];
		y = fbxMeshData->verts[index].normal[1];
		z = fbxMeshData->verts[index].normal[2];
		pos = FbxVector4(x, y, z);
		newPos = transformMatrix.MultNormalize(pos);
		newPos.Normalize();
		vertex.normal[0] = newPos.mData[0];
		vertex.normal[1] = newPos.mData[1];
		vertex.normal[2] = newPos.mData[2];

		vertex.uv = fbxMeshData->verts[index].uv;

		vertex.col = fbxMeshData->verts[index].col;
		oneFrame.verts.push_back(vertex);
	}
	return oneFrame;
}

void fbx_mesh::loadAnimation(double startTime, double endTime, double timeStep)
{
	// load each frame from fbx file and store it in a vector
	for (double time = startTime; time < endTime; time += timeStep) {
		this->fbx_mesh_all_fream.push_back(reFreshMeshWithCurrentTime(time));
	}
}

void fbx_mesh::obj_data_backup()
{/*
	for (int i = 0; i < this->fbxMeshData.size(); i++) {
		this->fbxMeshData[i].fbx_obj_data_backup.verts = this->fbxMeshData[i].fbx_obj_data.verts;
		this->fbxMeshData[i].fbx_obj_data_backup.v_n = this->fbxMeshData[i].fbx_obj_data.v_n;
		this->fbxMeshData[i].fbx_obj_data_backup.v_p = this->fbxMeshData[i].fbx_obj_data.v_p;
		this->fbxMeshData[i].fbx_obj_data_backup.v_t = this->fbxMeshData[i].fbx_obj_data.v_t;
	}*/
}

void fbx_mesh::loadFbxMeshToRender()
{/*
	this->vert_data.clear();
	for (int i = 0; i < this->fbxMeshData.size(); i++) {
		
		//load vert
		// Note : I Changed this to use the new vert_data array within Fyredon::Mesh - Niall (23/04/22) 
		for (int vr = 0; vr < this->fbxMeshData[i].fbx_obj_data.verts.size(); vr++) {
			Fyredon::Vert vert;
			vert.pos = this->fbxMeshData[i].fbx_obj_data.v_p[vr];
			vert.normal = this->fbxMeshData[i].fbx_obj_data.v_n[vr];
			vert.uv = this->fbxMeshData[i].fbx_obj_data.v_t[vr];
			this->vert_data.push_back(vert);
		}
	}
	set_data_mesh(this->vert_data);*/
}

void fbx_mesh::loadFbxMeshToRender(int FrameIndex)
{
	this->vert_data.clear();
	for (int mesh = 0; mesh < fbx_mesh_all_fream[FrameIndex].size(); mesh++) {
		for (int vertex = 0; vertex < fbx_mesh_all_fream[FrameIndex][mesh].verts.size(); vertex++) {
			/*
			std::cout << "size: " << this->vert_data.size() << 
				" mesh: "<< mesh <<"/"<< fbx_mesh_all_fream[FrameIndex].size()<<
				" vertex: " << vertex <<"/"<<fbx_mesh_all_fream[FrameIndex][mesh].verts.size() << ";"
				<<index<<";" << mesh * fbx_mesh_all_fream[FrameIndex][mesh].verts.size() + vertex << std::endl;
				*/
			Fyredon::Vert vex;
			vex.col[0] = fbx_mesh_all_fream[FrameIndex][mesh].verts[vertex].col[0];
			vex.col[1] = fbx_mesh_all_fream[FrameIndex][mesh].verts[vertex].col[1];
			vex.col[2] = fbx_mesh_all_fream[FrameIndex][mesh].verts[vertex].col[2];
			vex.pos[0] = fbx_mesh_all_fream[FrameIndex][mesh].verts[vertex].pos[0];
			vex.pos[1] = fbx_mesh_all_fream[FrameIndex][mesh].verts[vertex].pos[1];
			vex.pos[2] = fbx_mesh_all_fream[FrameIndex][mesh].verts[vertex].pos[2];
			vex.normal[0] = fbx_mesh_all_fream[FrameIndex][mesh].verts[vertex].normal[0];
			vex.normal[1] = fbx_mesh_all_fream[FrameIndex][mesh].verts[vertex].normal[1];
			vex.normal[2] = fbx_mesh_all_fream[FrameIndex][mesh].verts[vertex].normal[2];
			vex.uv[0] = fbx_mesh_all_fream[FrameIndex][mesh].verts[vertex].uv[0];
			vex.uv[1] = fbx_mesh_all_fream[FrameIndex][mesh].verts[vertex].uv[1];
			vex.uv[2] = fbx_mesh_all_fream[FrameIndex][mesh].verts[vertex].uv[2];
			this->vert_data.push_back(vex);
		}
	}
	/*
	std::cout << "<" << vert_data.size() << std::endl;
	if (this->vert_data.size() > 1e05) std::cout << "Info:: Auto-Indexing taking place on large mesh: " << name << ".\n";

	*/
}

