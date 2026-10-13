// Run from the project root. Requires the bundled Autodesk FBX SDK.
// Usage: transfer-mc-uv.exe [source.fbx candidate.fbx]. Reads Assets/Mesh/MC.obj; never overwrites the source.
#include <fbxsdk.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <cmath>
#include <stdexcept>
#include <string>
struct Obj { std::vector<FbxVector4> vertices; std::vector<FbxVector2> uv; std::vector<std::vector<int>> faces, indices; };
void require(bool ok,const char* message){if(!ok)throw std::runtime_error(message);}
int index(const std::string& s,int count){int i=std::stoi(s);i=i>0?i-1:count+i;require(i>=0&&i<count,"OBJ index out of range");return i;}
Obj readObj(){
 std::ifstream in("Assets/Mesh/MC.obj");require(bool(in),"Cannot read OBJ");Obj o;std::string line;
 while(std::getline(in,line)){
  std::istringstream row(line);std::string type;row>>type;
  if(type=="v"){double x,y,z;require(bool(row>>x>>y>>z),"Invalid vertex");o.vertices.emplace_back(x,y,z);}
  else if(type=="vt"){double u,v;require(bool(row>>u>>v),"Invalid UV");require(std::isfinite(u)&&std::isfinite(v),"Non-finite UV");o.uv.emplace_back(u,v);}
  else if(type=="f"){
   std::vector<int> f,t;std::string corner;
   while(row>>corner){if(corner[0]=='#')break;auto p=corner.find('/');require(p!=std::string::npos,"Missing UV");
    auto q=corner.find('/',p+1);auto uv=corner.substr(p+1,q==std::string::npos?q:q-p-1);require(!uv.empty(),"Missing UV");
    f.push_back(index(corner.substr(0,p),int(o.vertices.size())));t.push_back(index(uv,int(o.uv.size())));
   }require(f.size()>=3,"Invalid face");o.faces.push_back(f);o.indices.push_back(t);
  }
 }require(!o.uv.empty(),"OBJ has no UVs");return o;
}
FbxScene* load(FbxManager* mgr,const char* path){
 auto* scene=FbxScene::Create(mgr,"scene");auto* importer=FbxImporter::Create(mgr,"");
 require(importer->Initialize(path,-1,mgr->GetIOSettings()),"Cannot initialize FBX import");
 require(importer->Import(scene),"Cannot import FBX");importer->Destroy();return scene;
}
void nodes(FbxNode* n,std::vector<FbxNode*>& out){out.push_back(n);for(int i=0;i<n->GetChildCount();++i)nodes(n->GetChild(i),out);}
void matrix(FbxAMatrix a,FbxAMatrix b){for(int r=0;r<4;++r)for(int c=0;c<4;++c)require(std::abs(a[r][c]-b[r][c])<1e-8,"Transform changed");}
void verifyScene(FbxScene* a,FbxScene* b){
 std::vector<FbxNode*> na,nb;nodes(a->GetRootNode(),na);nodes(b->GetRootNode(),nb);require(na.size()==nb.size(),"Node count changed");
 int stacks=a->GetSrcObjectCount<FbxAnimStack>();require(stacks==b->GetSrcObjectCount<FbxAnimStack>(),"Animation count changed");
 int keysA=0,keysB=0;
 for(int i=0;i<a->GetSrcObjectCount<FbxAnimCurve>();++i)keysA+=a->GetSrcObject<FbxAnimCurve>(i)->KeyGetCount();
 for(int i=0;i<b->GetSrcObjectCount<FbxAnimCurve>();++i)keysB+=b->GetSrcObject<FbxAnimCurve>(i)->KeyGetCount();
 require(keysA==keysB,"Animation key count changed");
 for(int s=0;s<stacks;++s){
  auto* sa=a->GetSrcObject<FbxAnimStack>(s);auto* sb=b->GetSrcObject<FbxAnimStack>(s);
  require(std::string(sa->GetName())==sb->GetName(),"Clip name changed");
  a->SetCurrentAnimationStack(sa);b->SetCurrentAnimationStack(sb);
  auto span=sa->GetLocalTimeSpan();auto spanB=sb->GetLocalTimeSpan();
  require(span.GetStart()==spanB.GetStart()&&span.GetStop()==spanB.GetStop(),"Clip duration changed");
  for(int f=0;f<=30;++f){
   FbxTime t;t.SetSecondDouble(span.GetStart().GetSecondDouble()+span.GetDuration().GetSecondDouble()*f/30.0);
   for(size_t n=0;n<na.size();++n){
    require(std::string(na[n]->GetName())==nb[n]->GetName(),"Node hierarchy changed");
    matrix(na[n]->EvaluateGlobalTransform(t),nb[n]->EvaluateGlobalTransform(t));
   }
  }
 }
 int clusters=0;
 for(size_t n=0;n<na.size();++n){
  auto* ma=na[n]->GetMesh();auto* mb=nb[n]->GetMesh();require(bool(ma)==bool(mb),"Mesh node changed");if(!ma)continue;
  require(ma->GetControlPointsCount()==mb->GetControlPointsCount()&&ma->GetPolygonCount()==mb->GetPolygonCount(),"Topology changed");
  for(int i=0;i<ma->GetControlPointsCount();++i)for(int j=0;j<3;++j)require(std::abs(ma->GetControlPointAt(i)[j]-mb->GetControlPointAt(i)[j])<1e-10,"Geometry changed");
  for(int p=0;p<ma->GetPolygonCount();++p){
   require(ma->GetPolygonSize(p)==mb->GetPolygonSize(p),"Face changed");
   for(int c=0;c<ma->GetPolygonSize(p);++c)require(ma->GetPolygonVertex(p,c)==mb->GetPolygonVertex(p,c),"Face index changed");
  }
  int skins=ma->GetDeformerCount(FbxDeformer::eSkin);require(skins==mb->GetDeformerCount(FbxDeformer::eSkin),"Skin count changed");
  for(int i=0;i<skins;++i){
   auto* sa=static_cast<FbxSkin*>(ma->GetDeformer(i,FbxDeformer::eSkin));auto* sb=static_cast<FbxSkin*>(mb->GetDeformer(i,FbxDeformer::eSkin));
   require(sa->GetClusterCount()==sb->GetClusterCount()&&sa->GetSkinningType()==sb->GetSkinningType(),"Skin changed");
   for(int k=0;k<sa->GetClusterCount();++k){
    auto* ca=sa->GetCluster(k);auto* cb=sb->GetCluster(k);++clusters;
    require(std::string(ca->GetLink()->GetName())==cb->GetLink()->GetName()&&ca->GetLinkMode()==cb->GetLinkMode(),"Bone binding changed");
    require(ca->GetControlPointIndicesCount()==cb->GetControlPointIndicesCount(),"Weight count changed");
    for(int w=0;w<ca->GetControlPointIndicesCount();++w)require(ca->GetControlPointIndices()[w]==cb->GetControlPointIndices()[w]&&std::abs(ca->GetControlPointWeights()[w]-cb->GetControlPointWeights()[w])<1e-12,"Weight changed");
    FbxAMatrix x,y;ca->GetTransformMatrix(x);cb->GetTransformMatrix(y);matrix(x,y);ca->GetTransformLinkMatrix(x);cb->GetTransformLinkMatrix(y);matrix(x,y);
   }
  }
 }
 std::cout<<"Preserved: "<<na.size()<<" nodes, "<<clusters<<" skin clusters, "<<stacks<<" clips, "<<keysA<<" animation keys; 31 pose samples identical.\n";
}
int main(int argc,char** argv){try{
 const char* source=argc>1?argv[1]:"Assets/Mesh/MC.fbx";
 const char* output=argc>2?argv[2]:"Debug/uv-transfer/MC.updated.fbx";
 require(std::string(source)!=output,"Source and candidate paths must differ");
 auto obj=readObj();auto* mgr=FbxManager::Create();mgr->SetIOSettings(FbxIOSettings::Create(mgr,IOSROOT));
 auto* scene=load(mgr,source);std::vector<FbxNode*> all;nodes(scene->GetRootNode(),all);FbxMesh* mesh=nullptr;
 for(auto* node:all)if(node->GetMesh()){require(!mesh,"Expected one mesh");mesh=node->GetMesh();}
 require(mesh&&mesh->GetControlPointsCount()==obj.vertices.size()&&mesh->GetPolygonCount()==obj.faces.size(),"Mesh topology does not match OBJ");
 for(int i=0;i<mesh->GetControlPointsCount();++i)for(int j=0;j<3;++j)require(std::abs(mesh->GetControlPointAt(i)[j]-obj.vertices[i][j])<1e-5,"OBJ and FBX vertex ordering differ");
 for(int p=0;p<mesh->GetPolygonCount();++p){require(mesh->GetPolygonSize(p)==obj.faces[p].size(),"Face size mismatch");for(int c=0;c<mesh->GetPolygonSize(p);++c)require(mesh->GetPolygonVertex(p,c)==obj.faces[p][c],"Face order mismatch");}
 auto* uv=mesh->GetElementUV(0);if(!uv)uv=mesh->CreateElementUV("UVMap");
 uv->SetMappingMode(FbxGeometryElement::eByPolygonVertex);uv->SetReferenceMode(FbxGeometryElement::eIndexToDirect);
 uv->GetDirectArray().Clear();uv->GetIndexArray().Clear();for(auto v:obj.uv)uv->GetDirectArray().Add(v);
 int corners=0;for(auto face:obj.indices)for(auto i:face){uv->GetIndexArray().Add(i);++corners;}

 auto* exporter=FbxExporter::Create(mgr,"");require(exporter->Initialize(output,-1,mgr->GetIOSettings()),"Cannot initialize exporter");require(exporter->Export(scene),"FBX export failed");exporter->Destroy();
 auto* original=load(mgr,source);auto* saved=load(mgr,output);verifyScene(original,saved);
 std::vector<FbxNode*> savedNodes;nodes(saved->GetRootNode(),savedNodes);FbxMesh* savedMesh=nullptr;for(auto* n:savedNodes)if(n->GetMesh())savedMesh=n->GetMesh();
 auto* savedUV=savedMesh->GetElementUV(0);
 for(int p=0;p<savedMesh->GetPolygonCount();++p)for(int c=0;c<savedMesh->GetPolygonSize(p);++c){
  FbxVector2 v;bool unmapped=false;require(savedMesh->GetPolygonVertexUV(p,c,savedUV->GetName(),v,unmapped)&&!unmapped,"Missing exported UV");
  auto expected=obj.uv[obj.indices[p][c]];require(std::abs(v[0]-expected[0])<1e-10&&std::abs(v[1]-expected[1])<1e-10,"Exported UV mismatch");
 }
 std::cout<<"Verified "<<obj.uv.size()<<" OBJ UVs across "<<corners<<" face corners. Candidate: "<<output<<std::endl;mgr->Destroy();return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<std::endl;return 1;}}
