#pragma once
#include "Fyredon/Render/Mesh.h"
#include "FBXLoader.h"
#include <vector>
#include <string>

class fbx_mesh : public Fyredon::Mesh {
public:
    fbx_mesh(const char* name, const char* path);
    ~fbx_mesh();
    void load_obj(bool has_tex = false);
    void playCurrentFrame(int frameIndex = 0);
    void sample(double seconds);
    void selectClip(int index);
    const std::string& filePath() const { return source_path; }
    std::vector<std::string> clips;
    int clip = 0;
    double start_seconds = 0, duration = 0, fps = 30;
    FBXLoader* fbxloader = nullptr;
private:
    struct Part {
        FbxNode* node;
        std::vector<int> control_points;
        std::vector<Fyredon::Vert> corners;
    };
    std::string source_path;
    std::vector<Part> parts;
    bool uploaded = false;
    double last_sample = -1e30;
    void collect(FbxNode* node);
};
