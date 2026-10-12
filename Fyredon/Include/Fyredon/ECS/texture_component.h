#pragma once
#include "Fyredon/ECS/Component.h"
#include "Fyredon/ECS/mesh_component.h"
#include "Fyredon/Render/Texture.h"
#include <memory>

class texture_component : public Component {
public:
    explicit texture_component(mesh_component* mesh);
    void GUI_struct() override;
    void tick() override;
    // Texture resources cannot be backed up with Component's raw memcpy.
    void init() override {}
    void reset() override {}
private:
    mesh_component* owner;
    char path[1024] = {};
    glm::vec2 uv_offset = glm::vec2(0.f);
    glm::vec2 uv_scale = glm::vec2(1.f);
    std::string attempted_path;
    std::string error;
    std::shared_ptr<Fyredon::TextureImage> loaded;
    Fyredon::TextureImage* initial_texture = nullptr;
    void apply();
};
