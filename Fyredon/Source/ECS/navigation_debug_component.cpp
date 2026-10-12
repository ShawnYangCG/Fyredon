#include "Fyredon/Other/pch.h"
#include "Fyredon/ECS/navigation_debug_component.h"
#include "Fyredon/ECS/System/NavigationSystem.h"
#include <cmath>
navigation_debug_component::navigation_debug_component(NavigationSystem* source):system(source) {
    componentName="Debug";
    isActive=false;
}
void navigation_debug_component::GUI_struct() {
    ImGui::TextUnformatted("A* route: orange dashed rectangles");
    ImGui::Text("Dashes: %d",dashes);
    ImGui::TextWrapped("Shows the last computed route. Repath updates it; Reset or Reload map clears it.");
}
void navigation_debug_component::hide() {
    if(submitted){Fyredon::Renderer::get_renderer().remove_mesh(route);submitted=false;}
}
void navigation_debug_component::reset() {
    hide();lastMap=nullptr;revision=0;dashes=0;
}
void navigation_debug_component::tick() {
    auto* map=system&&system->map
        ? dynamic_cast<navigation_component*>(system->map->getComponentByName("Navigation")):nullptr;
    if(!isActive||!map||map->debugPath.empty()){hide();dashes=0;lastMap=nullptr;return;}
    if(lastMap!=map||revision!=map->pathRevision){
        rebuild(map);lastMap=map;revision=map->pathRevision;
    }
    if(dashes&&!submitted){Fyredon::Renderer::get_renderer().submit_mesh(route);submitted=true;}
}
void navigation_debug_component::rebuild(navigation_component* map) {
    hide();dashes=0;
    const float cell=map->width_grid/map->widthCount;
    const float dashLength=cell*0.22f, spacing=cell*0.34f, halfWidth=cell*0.045f;
    std::vector<Fyredon::Vert> vertices;
    for(size_t i=1;i<map->debugPath.size();++i){
        const auto start=map->debugPath[i-1],delta=map->debugPath[i]-start;
        const float length=glm::length(delta);
        if(length<1e-5f)continue;
        const auto forward=delta/length;
        const glm::vec2 side(-forward.y*halfWidth,forward.x*halfWidth);
        for(float t=0;t<length;t+=spacing){
            auto a=start+forward*t,b=start+forward*std::min(t+dashLength,length);
            glm::vec2 corners[]={a-side,b-side,b+side,a+side};
            for(int index:{0,2,1,0,3,2}){
                Fyredon::Vert v;
                v.pos=glm::vec3(corners[index].x,0.025f,corners[index].y);
                v.normal=glm::vec3(0,1,0);v.col=glm::vec3(1.f,0.5f,0.02f);v.uv=glm::vec3(0);
                vertices.push_back(v);
            }
            ++dashes;
        }
    }
    if(vertices.empty())return;
    if(!route){
        route=new Fyredon::Mesh("Navigation debug route");
        route->set_shader("../Shaders/mesh.vert","../Shaders/mesh.frag");
    }
    route->set_data_mesh(vertices);
}
