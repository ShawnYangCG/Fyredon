#include "Fyredon/Other/pch.h"
#include "Fyredon/ECS/fbx_property_component.h"
#include "Fyredon/ECS/mesh_component.h"
#include "Fyredon/FBX/fbx_mesh.h"
#include <algorithm>
#include <cmath>
fbx_property_component::fbx_property_component(){componentName="Animation";}
fbx_property_component::fbx_property_component(float,float,float):fbx_property_component(){}
fbx_mesh* fbx_property_component::mesh(){
    auto* c=static_cast<mesh_component*>(parent->getComponentByName("Mesh"));
    return c?dynamic_cast<fbx_mesh*>(c->mesh):nullptr;
}
void fbx_property_component::configure(){
    if(configured || !mesh())return;
    startTime=0;endTime=static_cast<float>(std::round(mesh()->duration*mesh()->fps));
    cursor=0;currentFrameIndex=0;lastTick=glfwGetTime();configured=true;
}
void fbx_property_component::GUI_struct(){
    configure();auto* m=mesh();if(!m)return;
    if(!m->clips.empty() && ImGui::BeginCombo("Clip",m->clips[m->clip].c_str())){
        for(int i=0;i<static_cast<int>(m->clips.size());++i)if(ImGui::Selectable(m->clips[i].c_str(),m->clip==i)){
            m->selectClip(i);configured=false;configure();
        }
        ImGui::EndCombo();
    }
    ImGui::Text("Duration: %.3f s | FPS: %.2f",m->duration,m->fps);
    ImGui::SliderFloat("Speed",&speed,0.f,4.f);
    ImGui::InputFloat("Start frame",&startTime,1.f);
    ImGui::InputFloat("End frame",&endTime,1.f);
    checkValid();
    if(navigationControlled) ImGui::TextUnformatted(isPlay ? "Navigation: walking" : "Navigation: stopped");
    else if(ImGui::Button(isPlay?"Pause":"Play")){if(isPlay)stop();else play();}
    ImGui::SameLine();if(ImGui::Button("Reset"))reset();
    if(ImGui::SliderInt("Current frame",&currentFrameIndex,static_cast<int>(startTime),static_cast<int>(endTime))){
        cursor=currentFrameIndex;lastTick=glfwGetTime();m->playCurrentFrame(currentFrameIndex);
    }
}
void fbx_property_component::checkValid(){
    auto* m=mesh();if(!m)return;
    const float maxFrame=static_cast<float>(std::round(m->duration*m->fps));
    startTime=std::max(0.f,std::min(startTime,maxFrame));
    endTime=std::max(startTime,std::min(endTime,maxFrame));
    speed=std::max(0.f,std::min(speed,4.f));
    cursor=std::max(static_cast<double>(startTime),std::min(cursor,static_cast<double>(endTime)));
}
void fbx_property_component::tick(){playAnimation();}
void fbx_property_component::play(){if(navigationControlled)return;configure();if(!isPlay)lastTick=glfwGetTime();isPlay=true;}
void fbx_property_component::stop(){isPlay=false;lastTick=glfwGetTime();}
void fbx_property_component::reset(){configure();stop();cursor=startTime;currentFrameIndex=static_cast<int>(cursor);if(mesh())mesh()->playCurrentFrame(currentFrameIndex);}
void fbx_property_component::setPlay(){play();}
void fbx_property_component::playAnimation(){
    configure();auto* m=mesh();if(!m)return;checkValid();
    double now=glfwGetTime();double dt=std::max(0.0,now-lastTick);lastTick=now;
    if(isPlay && isActive && endTime>startTime){
        cursor+=dt*m->fps*speed;
        cursor=startTime+std::fmod(cursor-startTime,endTime-startTime);
    }
    currentFrameIndex=static_cast<int>(cursor);
    m->sample(m->start_seconds+std::min(cursor/m->fps,m->duration));
}

void fbx_property_component::setNavigationControlled(bool enabled)
{
    navigationControlled=enabled;
    stop();
}
void fbx_property_component::setNavigating(bool moving)
{
    if(!navigationControlled)return;
    configure();
    if(isPlay!=moving)lastTick=glfwGetTime();
    isPlay=moving;
}
void fbx_property_component::meshChanged()
{
    stop();configured=false;startTime=endTime=0;cursor=0;currentFrameIndex=0;
    configure();
}