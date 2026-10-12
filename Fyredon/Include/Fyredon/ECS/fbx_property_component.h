#pragma once
#include "Fyredon/ECS/Component.h"
class fbx_mesh;
class fbx_property_component : public Component {
public:
    fbx_property_component();
    fbx_property_component(float startTime,float endTime,float speed);
    float speed=1.f, startTime=0.f, endTime=0.f;
    int currentFrameIndex=0;
    void GUI_struct() override;
    void tick() override;
    void play() override;
    void stop() override;
    void reset() override;
    void init() override {}
    void playAnimation();
    void checkValid();
    void setPlay();
    void meshChanged();
    void setNavigationControlled(bool enabled);
    void setNavigating(bool moving);
    bool isPlaying() const { return isPlay; }
private:
    fbx_mesh* mesh();
    bool isPlay=false, configured=false;
    bool navigationControlled=false;
    double cursor=0, lastTick=0;
    void configure();
};
