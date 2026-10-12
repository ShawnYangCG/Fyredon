#include "Fyredon/Other/pch.h"
#include "Fyredon/ECS/texture_component.h"
#include <filesystem>
#include <cstring>

texture_component::texture_component(mesh_component* mesh) : owner(mesh) {
    componentName = "Texture";
    initial_texture = owner->mesh->tex;
    uv_offset = owner->mesh->uv_offset;
    uv_scale = owner->mesh->uv_scale;
    isActive = owner->mesh->use_tex && initial_texture;
    if (initial_texture) {
        auto text = initial_texture->file_path;
        strncpy_s(path, sizeof(path), text.c_str(), _TRUNCATE);
        attempted_path = path;
    }
}
void texture_component::GUI_struct() {
    ImGui::InputText("File Path", path, sizeof(path));
    if (ImGui::IsItemDeactivatedAfterEdit()) { attempted_path.clear(); apply(); }
    if (isActive && !error.empty()) ImGui::TextWrapped("%s", error.c_str());
    ImGui::DragFloat2("UV Offset", &uv_offset.x, 0.01f);
    ImGui::DragFloat2("UV Scale", &uv_scale.x, 0.01f);
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("U / V repeats: 1 = original, 2 = two repeats. Negative values mirror.");
    if (ImGui::Button("Reset UV")) {
        uv_offset = glm::vec2(0.f);
        uv_scale = glm::vec2(1.f);
    }
}
void texture_component::tick() { apply(); }
void texture_component::apply() {
    if (isActive && attempted_path != path && !ImGui::IsAnyItemActive()) {
        attempted_path = path;
        error.clear();
        try {
            auto file = std::filesystem::u8path(path);
            if (file.empty()) throw std::runtime_error("Enter an image path.");
            if (file.is_relative()) {
                const auto root = std::filesystem::current_path().parent_path();
                if (std::filesystem::is_regular_file(root / file)) file = root / file;
                else file = std::filesystem::absolute(file);
            }
            if (!std::filesystem::is_regular_file(file)) throw std::runtime_error("Image file not found.");
            auto candidate = std::make_shared<Fyredon::TextureImage>(file.u8string().c_str(), "Object texture", 0, true);
            candidate->load();
            candidate->set_params(Fyredon::Texture::filter_type::LINEAR);
            // Point both render copies to the new image before releasing the old one.
            owner->mesh->tex = owner->mesh_render->tex = candidate.get();
            loaded = candidate;
        } catch (const std::exception& e) { error = e.what(); }
    }
    auto* texture = loaded ? loaded.get() : initial_texture;
    const bool enabled = isActive && texture && error.empty() && path[0];
    for (auto* mesh : {owner->mesh, owner->mesh_render}) {
        mesh->tex = texture;
        mesh->uv_offset = uv_offset;
        mesh->uv_scale = uv_scale;
        mesh->use_tex = enabled;
        mesh->flags.texture_set = enabled;
    }
    if (isActive && !path[0]) error = "Enter an image path.";

}
