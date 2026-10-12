#include "Fyredon/Other/pch.h"
#include "Fyredon/ECS/World.h"
#include "Fyredon/ECS/navigation_component.h"
#include "Fyredon/ECS/fbx_entity.h"
#include "Fyredon/ECS/texture_component.h"
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <stdexcept>
void check(bool ok){if(!ok)throw std::runtime_error("Reset regression check failed");}
int main(){
 try {
  check(glfwInit()!=0);glfwWindowHint(GLFW_VISIBLE,GLFW_FALSE);
  auto* window=glfwCreateWindow(64,64,"Reset test",nullptr,nullptr);check(window!=nullptr);
  glfwMakeContextCurrent(window);check(glewInit()==GLEW_OK);ImGui::CreateContext();
  auto* world=new World(nullptr);
  auto* floor=new Entity("floor");floor->addTransformComponent();
  auto* mesh=new Fyredon::Mesh_OBJ("floor","../Assets/Mesh/plane5x5.obj");
  mesh->load_obj();mesh->set_shader("../Shaders/mesh.vert","../Shaders/mesh.frag");
  mesh->load_texture("../Assets/Images/Terrain/RPG_grass_pixel.png",0);
  auto* mc=new mesh_component(mesh);floor->addComponent(mc);
  auto* nav=new navigation_component(floor);floor->addComponent(nav);nav->createGrid();
  world->addEntity(floor);
  auto* human=new fbx_entity("human","../Assets/Mesh/MC.fbx");
  human->init("../Shaders/human.vert","../Shaders/human.frag",0,0,0,glm::vec3(0),"../Assets/Images/human/MC.png");
  world->addEntity(human);
  auto* tr=static_cast<transform_component*>(human->getComponentByName("Transform"));
  auto* tex=static_cast<texture_component*>(human->getComponentByName("Texture"));
  auto* hm=static_cast<mesh_component*>(human->getComponentByName("Mesh"));
  auto* texture=hm->mesh->tex;auto* grid=nav->map;
  tr->matrixScale[0]=tr->matrixScale[1]=tr->matrixScale[2]=20.f;
  world->reset(); // No snapshot yet: must be safe.
  for(int cycle=0;cycle<100;++cycle){
   tr->matrixTranslation[0]=0;
   world->init();world->play();
   nav->path=nav->findPath(glm::vec2(-3,-3),glm::vec2(3,3));
   check(!nav->path.empty());nav->path.pop_front();
   tr->matrixTranslation[0]=2.f;human->FBXmesh->playCurrentFrame(15);
   world->stop();world->init();world->play(); // Resume must not replace original snapshot.
   tr->matrixTranslation[0]=3.f;
   world->reset();world->reset();
   check(tr->matrixTranslation[0]==0&&tr->matrixScale[0]==20.f);
   check(nav->path.empty()&&nav->posList==nullptr&&nav->map==grid);
   check(hm->mesh->tex==texture&&tex->isActive);
   human->FBXmesh->playCurrentFrame(20);human->FBXmesh->playCurrentFrame(0);
   nav->path=nav->findPath(glm::vec2(3,3),glm::vec2(-3,-3));
   check(!nav->path.empty()); // Replaying navigation after reset remains valid.
  }
  tr->matrixTranslation[0]=5;world->init();tr->matrixTranslation[0]=9;world->reset();
  check(tr->matrixTranslation[0]==5);
  std::cout<<"PASS: reset before play, 100 play/pause/resume/reset cycles, repeated reset, navigation reuse, animation, textures, new session snapshot."<<std::endl;
  return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<std::endl;return 1;}
}
