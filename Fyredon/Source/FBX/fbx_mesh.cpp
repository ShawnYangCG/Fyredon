#include "Fyredon/Other/pch.h"
#include "Fyredon/FBX/fbx_mesh.h"
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <cstdlib>

fbx_mesh::fbx_mesh(const char* name, const char* path) : Mesh(name), source_path(path) {
    fbxloader = new FBXLoader(source_path.c_str());
    if (!fbxloader->LoadFile()) { delete fbxloader; fbxloader=nullptr; throw std::runtime_error("FBX import failed: " + source_path); }
}
fbx_mesh::~fbx_mesh() { delete fbxloader; }
void fbx_mesh::collect(FbxNode* node) {
    if (auto* mesh = node->GetMesh()) {
        Part part; part.node = node;
        FbxStringList uvSets; mesh->GetUVSetNames(uvSets);
        for (int p = 0; p < mesh->GetPolygonCount(); ++p) {
            if (mesh->GetPolygonSize(p) != 3) throw std::runtime_error("FBX triangulation failed");
            for (int c = 0; c < 3; ++c) {
                int cp = mesh->GetPolygonVertex(p, c);
                if (cp < 0 || cp >= mesh->GetControlPointsCount()) throw std::runtime_error("Invalid FBX control point");
                part.control_points.push_back(cp);
                Fyredon::Vert v; v.pos = glm::vec3(0); v.normal = glm::vec3(0,1,0);
                v.col = glm::vec3(1); v.uv = glm::vec3(0);
                if (uvSets.GetCount()) {
                    FbxVector2 uv(0,0); bool unmapped = false;
                    if (mesh->GetPolygonVertexUV(p,c,uvSets[0],uv,unmapped) && !unmapped)
                        v.uv = glm::vec3(uv[0],uv[1],0);
                }
                part.corners.push_back(v);
            }
        }
        if (!part.corners.empty()) parts.push_back(std::move(part));
    }
    for (int i=0;i<node->GetChildCount();++i) collect(node->GetChild(i));
}
void fbx_mesh::load_obj(bool) {
    auto* scene = fbxloader->Scene;
    // FBXLoader normalizes imported coordinates to centimeters; render in meters.
    collect(scene->GetRootNode());
    if (parts.empty()) throw std::runtime_error("FBX contains no meshes");
    for (int i=0;i<scene->GetSrcObjectCount<FbxAnimStack>();++i)
        clips.push_back(scene->GetSrcObject<FbxAnimStack>(i)->GetName());
    fps = FbxTime::GetFrameRate(scene->GetGlobalSettings().GetTimeMode());
    if (!std::isfinite(fps) || fps <= 0) fps = 30;
    selectClip(0);
    sample(start_seconds);
    int clusters = 0;
    for (auto& part : parts) {
        auto* mesh=part.node->GetMesh();
        for(int i=0;i<mesh->GetDeformerCount(FbxDeformer::eSkin);++i)
            clusters += static_cast<FbxSkin*>(mesh->GetDeformer(i,FbxDeformer::eSkin))->GetClusterCount();
    }
    glm::vec3 lo=vert_data[0].pos, hi=lo;
    for(const auto& v:vert_data){lo=glm::min(lo,v.pos);hi=glm::max(hi,v.pos);}
    std::cout << "FBX animation: " << source_path << " meshes=" << parts.size() << " clusters=" << clusters
        << " clips=" << clips.size() << " duration=" << duration << " fps=" << fps
        << " bounds=" << hi.x-lo.x << "," << hi.y-lo.y << "," << hi.z-lo.z << std::endl;
    // Optional import regression check: sample the whole clip, including its endpoint.
    if (std::getenv("FYREDON_VALIDATE_FBX")) {
        const auto initial = vert_data;
        double maxDeformation = 0;
        for (int frame=0; frame<=static_cast<int>(std::ceil(duration*fps)); ++frame) {
            playCurrentFrame(frame);
            if (vert_data.size()!=initial.size()) throw std::runtime_error("Animation topology changed");
            const auto translation=vert_data[0].pos-initial[0].pos;
            for (std::size_t i=0;i<vert_data.size();++i) {
                const auto p=vert_data[i].pos;
                if(!std::isfinite(p.x)||!std::isfinite(p.y)||!std::isfinite(p.z))
                    throw std::runtime_error("Non-finite skinned position");
                maxDeformation=std::max(maxDeformation,static_cast<double>(
                    glm::length((p-initial[i].pos)-translation)));
            }
        }
        sample(start_seconds);
        std::cout << "FBX validation: finite vertices, stable topology, relative deformation="
                  << maxDeformation << " m" << std::endl;
    }
}
void fbx_mesh::selectClip(int index) {
    auto* scene=fbxloader->Scene;
    if (!clips.empty()) {
        clip=std::max(0,std::min(index,static_cast<int>(clips.size())-1));
        auto* stack=scene->GetSrcObject<FbxAnimStack>(clip);
        scene->SetCurrentAnimationStack(stack);
        fbxloader->CurrentAnimLayer=stack->GetMember<FbxAnimLayer>();
        auto span=stack->GetLocalTimeSpan();
        if(auto* take=scene->GetTakeInfo(stack->GetName())) span=take->mLocalTimeSpan;
        start_seconds=span.GetStart().GetSecondDouble();
        duration=std::max(0.0,span.GetDuration().GetSecondDouble());
    }
    scene->GetAnimationEvaluator()->Reset();
    last_sample=-1e30;
}
void fbx_mesh::playCurrentFrame(int frame) {
    sample(start_seconds+std::max(0.0,std::min(static_cast<double>(frame)/fps,duration)));
}
void fbx_mesh::sample(double seconds) {
    if(uploaded && seconds==last_sample) return;
    FbxTime time; time.SetSecondDouble(seconds);
    std::vector<Fyredon::Vert> vertices;
    for (auto& part:parts) {
        auto* mesh=part.node->GetMesh();
        std::vector<FbxVector4> points(mesh->GetControlPoints(),mesh->GetControlPoints()+mesh->GetControlPointsCount());
        auto global=part.node->EvaluateGlobalTransform(time)*fbxloader->GetGeometry(part.node);
        if(mesh->GetShapeCount() && fbxloader->CurrentAnimLayer)
            fbxloader->ComputeShapeDeformation(mesh,time,fbxloader->CurrentAnimLayer,points.data());
        int clusters=0;
        for(int i=0;i<mesh->GetDeformerCount(FbxDeformer::eSkin);++i)
            clusters+=static_cast<FbxSkin*>(mesh->GetDeformer(i,FbxDeformer::eSkin))->GetClusterCount();
        if(clusters) fbxloader->ComputeSkinDeformation(global,mesh,time,points.data(),nullptr);
        auto corners=part.corners;
        for(std::size_t i=0;i<corners.size();++i) {
            auto p=global.MultT(points[part.control_points[i]]);
            corners[i].pos=glm::vec3(p[0],p[1],p[2])*0.01f;
        }
        // Recompute deformed face normals, preserving the block model's hard edges.
        for(std::size_t i=0;i<corners.size();i+=3) {
            auto n=glm::cross(corners[i+1].pos-corners[i].pos,corners[i+2].pos-corners[i].pos);
            if(glm::length(n)>1e-9f) n=glm::normalize(n); else n=glm::vec3(0,1,0);
            corners[i].normal=corners[i+1].normal=corners[i+2].normal=n;
        }
        vertices.insert(vertices.end(),corners.begin(),corners.end());
    }
    if(!uploaded) { set_data_mesh(vertices, false); uploaded=true; }
    else { update_data_mesh(vertices); vert_data=std::move(vertices); }
    last_sample=seconds;
}
