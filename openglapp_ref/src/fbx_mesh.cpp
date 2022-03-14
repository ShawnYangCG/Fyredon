#include "fbx_mesh.h"

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
		std::cout << "Child node count: " << lRootNode->GetChildCount() << std::endl;
		for (int i = 0; i < lRootNode->GetChildCount(); i++)
			this->fbxloader->PrintNode(lRootNode->GetChild(i));
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
					FbxMesh *nMesh = pNode->GetMesh();
					VBOMesh vboMesh = VBOMesh();
					vboMesh.Initialize(nMesh);
					std::cout << "Vertices count: " << vboMesh.lVerticesLength;
					std::cout << " Normals count:" << vboMesh.lNormalsLength;
					std::cout << " UV count:" << vboMesh.lUVsLength << std::endl;
					// add position
					for (int vertice = 0; vertice < vboMesh.lVerticesLength; vertice+=4) {
						this->obj_data.v_p.emplace_back(
							vboMesh.lVertices[vertice], vboMesh.lVertices[vertice + 1], vboMesh.lVertices[vertice + 2]);
					}
					
					// add normal
					for (int normal = 0; normal < vboMesh.lNormalsLength; normal += 3) {
						this->obj_data.v_n.emplace_back(
							vboMesh.lNormals[normal], vboMesh.lNormals[normal + 1], vboMesh.lNormals[normal + 2]);
					}
					
					// add uv
					for (int uv = 0; uv < vboMesh.lUVsLength; uv += 2) {
						this->obj_data.v_t.emplace_back(
							vboMesh.lUVs[uv], vboMesh.lUVs[uv + 1]);
					}
					
					// add vert
					// Create Vertex
					for (int vertice = 0; vertice < vboMesh.verticesCount; vertice++) {
						vert vertex;
						vertex.pos = obj_data.v_p[vertice];
						vertex.normal = obj_data.v_n[vertice];
						vertex.uv = obj_data.v_t[vertice];
						vertex.col = glm::vec3(1.f, 1.f, 1.f);

						// Append to vert array
						obj_data.verts.push_back(std::move(vertex));
					}
				}
			}		
		}
	}
	obj_data_backup();
	set_data_mesh(obj_data.verts);
}

void fbx_mesh::reFreshMeshWithCurrentFrame(double time)
{
	FbxScene* lScene = this->fbxloader->Scene;
	FbxNode* lRootNode = lScene->GetRootNode();
	FbxAnimEvaluator* mySceneEvaluator = this->fbxloader->Scene->GetAnimationEvaluator();
	FbxTime myTime;       // The time for each key in the animation curve(s)
	myTime.SetSecondDouble(time);   // Starting time
	FbxNode* myMeshNode = lRootNode->GetChild(0);

	// Get a reference to node¡¯s global transform.
	//FbxMatrix GlobalMatrix = mySceneEvaluator->GetNodeGlobalTransform(myMeshNode, myTime);
	// Get a reference to node¡¯s local transform.
	FbxMatrix LocalMatrix = mySceneEvaluator->GetNodeLocalTransform(myMeshNode, myTime);

	refreshTransform(LocalMatrix);
	set_data_mesh(obj_data.verts);
}

void fbx_mesh::refreshTransform(FbxMatrix transformMatrix)
{
	for (int index = 0; index < this->obj_data.verts.size(); index++) {
		// refresh pos
		double x = this->obj_data_ori.verts[index].pos[0];
		double y = this->obj_data_ori.verts[index].pos[1];
		double z = this->obj_data_ori.verts[index].pos[2];
		FbxVector4 pos = FbxVector4(x, y, z);
		FbxVector4 newPos = transformMatrix.MultNormalize(pos);
		this->obj_data.verts[index].pos[0] = newPos.mData[0];
		this->obj_data.verts[index].pos[1] = newPos.mData[1];
		this->obj_data.verts[index].pos[2] = newPos.mData[2];
		
		// refresh normal
		x = this->obj_data_ori.verts[index].normal[0];
		y = this->obj_data_ori.verts[index].normal[1];
		z = this->obj_data_ori.verts[index].normal[2];
		pos = FbxVector4(x, y, z);
		newPos = transformMatrix.MultNormalize(pos);
		newPos.Normalize();
		this->obj_data.verts[index].normal[0] = newPos.mData[0];
		this->obj_data.verts[index].normal[1] = newPos.mData[1];
		this->obj_data.verts[index].normal[2] = newPos.mData[2];
	}
}

void fbx_mesh::obj_data_backup()
{
	obj_data_ori.verts = obj_data.verts;
	obj_data_ori.v_n = obj_data.v_n;
	obj_data_ori.v_p = obj_data.v_p;
	obj_data_ori.v_t = obj_data.v_t;
}

