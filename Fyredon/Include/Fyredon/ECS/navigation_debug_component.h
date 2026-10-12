#pragma once
#include "Fyredon/ECS/Component.h"
#include "Fyredon/Render/Mesh.h"
class NavigationSystem;
class navigation_component;
// Visualizes the last computed A* route, independently of its consumed movement queue.
class navigation_debug_component : public Component {
public:
    explicit navigation_debug_component(NavigationSystem* system);
    void GUI_struct() override;
    void tick() override;
    void init() override {}
    void reset() override;
    bool visible() const { return submitted; }
    int dashCount() const { return dashes; }
private:
    NavigationSystem* system;
    navigation_component* lastMap=nullptr;
    unsigned long long revision=0;
    Fyredon::Mesh* route=nullptr;
    bool submitted=false;
    int dashes=0;
    void hide();
    void rebuild(navigation_component* map);
};
