#include "Fyredon/Other/pch.h"
#include "Fyredon/FBX/fbx_mesh.h"

fbx_mesh::fbx_mesh(const char* name, const char* filePath)
	:Mesh(name, filePath)
{
	this->fbxloader = new FBXLoader(filePath);
	this->fbxloader->LoadFile();
}

fbx_mesh::~fbx_mesh()
{
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
					fbxObjectData fbxData = fbxObjectData();
					fbxData.pNode = pNode;
					FbxMesh *nMesh = pNode->GetMesh();
					VBOMesh vboMesh = VBOMesh();
					vboMesh.Initialize(nMesh);
					//std::cout << "Vertices count: " << vboMesh.lVerticesLength;
					//std::cout << " Normals count:" << vboMesh.lNormalsLength;
					//std::cout << " UV count:" << vboMesh.lUVsLength << std::endl;
					// add position
					for (int vertice = 0; vertice < vboMesh.lVerticesLength; vertice+=4) {
						fbxData.fbx_obj_data.v_p.emplace_back(
							vboMesh.lVertices[vertice], vboMesh.lVertices[vertice + 1], vboMesh.lVertices[vertice + 2]);
					}
					
					// add normal
					for (int normal = 0; normal < vboMesh.lNormalsLength; normal += 3) {
						fbxData.fbx_obj_data.v_n.emplace_back(
							vboMesh.lNormals[normal], vboMesh.lNormals[normal + 1], vboMesh.lNormals[normal + 2]);
					}
					
					// add uv
					for (int uv = 0; uv < vboMesh.lUVsLength; uv += 2) {
						fbxData.fbx_obj_data.v_t.emplace_back(
							vboMesh.lUVs[uv], vboMesh.lUVs[uv + 1], 0.f);
					}
					
					// add Vert
					// Create Vertex
					for (int Vertice = 0; Vertice < vboMesh.verticesCount; Vertice++) {
						Fyredon::Vert vertex;
						vertex.pos = fbxData.fbx_obj_data.v_p[Vertice];
						vertex.normal = fbxData.fbx_obj_data.v_n[Vertice];
						vertex.uv = fbxData.fbx_obj_data.v_t[Vertice];
						vertex.col = glm::vec3(1.f, 1.f, 1.f);

						// Append to vert array
						fbxData.fbx_obj_data.verts.push_back(std::move(vertex));
					}
					this->fbxMeshData.push_back(fbxData);
				}
			}		
		}
	}
	obj_data_backup();
	//set_data_mesh(obj_data.verts);
	reFreshMeshWithCurrentFrame(0.0);
	loadFbxMeshToRender();

	//************************************************************************************************
	// this function need to improve by get the frame number from fbx file, 0.02 is relate to the fps
	//************************************************************************************************
	loadAnimation(0.0, 1.0, 0.02);
}

void fbx_mesh::reFreshMeshWithCurrentTime(double time)
{
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

		refreshTransform(&this->fbxMeshData[i], LocalMatrix);
	}	
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
}

void fbx_mesh::refreshTransform(fbxObjectData* fbxMeshData, FbxMatrix transformMatrix)
{
	for (int index = 0; index < fbxMeshData->fbx_obj_data.verts.size(); index++) {
		// refresh pos
		double x = fbxMeshData->fbx_obj_data_backup.verts[index].pos[0];
		double y = fbxMeshData->fbx_obj_data_backup.verts[index].pos[1];
		double z = fbxMeshData->fbx_obj_data_backup.verts[index].pos[2];
		FbxVector4 pos = FbxVector4(x, y, z);
		FbxVector4 newPos = transformMatrix.MultNormalize(pos);
		fbxMeshData->fbx_obj_data.verts[index].pos[0] = newPos.mData[0];
		fbxMeshData->fbx_obj_data.verts[index].pos[1] = newPos.mData[1];
		fbxMeshData->fbx_obj_data.verts[index].pos[2] = newPos.mData[2];
		
		// refresh normal
		x = fbxMeshData->fbx_obj_data_backup.verts[index].normal[0];
		y = fbxMeshData->fbx_obj_data_backup.verts[index].normal[1];
		z = fbxMeshData->fbx_obj_data_backup.verts[index].normal[2];
		pos = FbxVector4(x, y, z);
		newPos = transformMatrix.MultNormalize(pos);
		newPos.Normalize();
		fbxMeshData->fbx_obj_data.verts[index].normal[0] = newPos.mData[0];
		fbxMeshData->fbx_obj_data.verts[index].normal[1] = newPos.mData[1];
		fbxMeshData->fbx_obj_data.verts[index].normal[2] = newPos.mData[2];
	}
}

void fbx_mesh::loadAnimation(double startTime, double endTime, double timeStep)
{
	// load each frame from fbx file and store it in a vector
	for (double time = startTime; time < endTime; time += timeStep) {
		reFreshMeshWithCurrentTime(time);
		this->fbx_mesh_all_fream.push_back(this->fbxMeshData);
	}
}

void fbx_mesh::obj_data_backup()
{
	for (int i = 0; i < this->fbxMeshData.size(); i++) {
		this->fbxMeshData[i].fbx_obj_data_backup.verts = this->fbxMeshData[i].fbx_obj_data.verts;
		this->fbxMeshData[i].fbx_obj_data_backup.v_n = this->fbxMeshData[i].fbx_obj_data.v_n;
		this->fbxMeshData[i].fbx_obj_data_backup.v_p = this->fbxMeshData[i].fbx_obj_data.v_p;
		this->fbxMeshData[i].fbx_obj_data_backup.v_t = this->fbxMeshData[i].fbx_obj_data.v_t;
	}
}

void fbx_mesh::loadFbxMeshToRender()
{
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
	set_data_mesh(this->vert_data);
}

void fbx_mesh::loadFbxMeshToRender(int FrameIndex)
{
	this->vert_data.clear();

	std::vector<fbxObjectData> oneFrame = fbx_mesh_all_fream[FrameIndex];
	for (int i = 0; i < oneFrame.size(); i++) {

		// Load Vert
		// Note : I Changed this to use the new vert_data array within Fyredon::Mesh - Niall (23/04/22) 
		for (int vr = 0; vr < oneFrame[i].fbx_obj_data.verts.size(); vr++) 
		{
			Fyredon::Vert vert;
			vert.pos = this->fbxMeshData[i].fbx_obj_data.v_p[vr];
			vert.normal = this->fbxMeshData[i].fbx_obj_data.v_n[vr];
			vert.uv = this->fbxMeshData[i].fbx_obj_data.v_t[vr];
			this->vert_data.push_back(vert);
		}
	}
	set_data_mesh(this->vert_data);
}

