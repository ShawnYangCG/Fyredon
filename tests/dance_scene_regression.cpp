#include "Fyredon/Other/pch.h"
#include "Fyredon/Action/MenuAction.h"
#include "Fyredon/GUI/GUI_RendererSettings.h"
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
void check(bool ok){if(!ok)throw std::runtime_error("Dance scene check failed");}
int main(){try {
 check(glfwInit()!=0);glfwWindowHint(GLFW_VISIBLE,GLFW_FALSE);
 auto* window=glfwCreateWindow(64,64,"Dance scene test",nullptr,nullptr);check(window!=nullptr);
 glfwMakeContextCurrent(window);check(glewInit()==GLEW_OK);ImGui::CreateContext();
 auto* panels=new GUI_PanelOrgnizer();
 auto* settings=new Fyredon::GUI_RendererSettings();panels->gui_panels["setting"]=settings;
 auto* world=new World(panels);auto* action=new Fyredon::MenuAction(panels,world);
 action->createDanceDemo();
 check(world->entities.size()==6&&!settings->state.draw_grid&&!settings->state.draw_axis);
 auto* ground=world->getEntityByName("Dance ground");
 check(ground&&ground->getComponentByName("Texture"));
 for(int i=1;i<=4;++i){
  auto* rock=world->getEntityByName("rock"+std::to_string(i));
  check(rock&&rock->getComponentByName("Mesh")&&rock->getComponentByName("Transform"));
 }
 auto* dancer=dynamic_cast<fbx_entity*>(world->getEntityByName("MC_dance"));
 check(dancer&&dancer->FBXmesh->duration>0&&!dancer->getComponentByName("Move test"));
 auto* animation=dynamic_cast<fbx_property_component*>(dancer->getComponentByName("Animation"));check(animation!=nullptr);
 auto initial=dancer->FBXmesh->vert_data;
 float movement=0;
 for(int frame=1;frame<=20;++frame){
  dancer->FBXmesh->sample(dancer->FBXmesh->start_seconds+dancer->FBXmesh->duration*frame/20.0);
  check(initial.size()==dancer->FBXmesh->vert_data.size());
  for(size_t i=0;i<initial.size();++i)movement=std::max(movement,glm::length(initial[i].pos-dancer->FBXmesh->vert_data[i].pos));
 }
 check(movement>0.0001f);
 world->init();world->play();glfwSetTime(glfwGetTime()+0.2);animation->tick();check(animation->isPlaying());
 world->stop();check(!animation->isPlaying());world->reset();check(animation->currentFrameIndex==0);
 std::cout<<"PASS: six independent entities, grass texture, four rocks, animated dance FBX, Play/Pause/Reset. Vertex displacement="<<movement<<std::endl;
 return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<std::endl;return 1;}}
