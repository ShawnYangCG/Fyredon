#include "Fyredon/Other/pch.h"
#include "Fyredon/ECS/World.h"
#include "Fyredon/ECS/navigation_component.h"
#include "Fyredon/ECS/fbx_entity.h"
#include "Fyredon/Navigation/NavigationMapFile.h"
#include "Fyredon/ECS/System/NavigationSystem.h"
#include "Fyredon/ECS/navigation_entity.h"
#include "Fyredon/ECS/navigation_debug_component.h"
#include <filesystem>
#include "Fyredon/ECS/texture_component.h"
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
void check(bool ok){if(!ok)throw std::runtime_error("Map regression failed");}
void write(const std::string& s){std::ofstream("map-test.txt",std::ios::binary)<<s;}
int main(){try{
 auto valid=parseNavigationMap("\xEF\xBB\xBF 2,\r\n0; \n0,3;\r\n");check(valid.cells.size()==2);
 for(auto text:{"", "2,0\n0,3", "2,0;3", "2,0;0,4", "2,2;0,3", "2,,0;0,0,0;0,0,3", "2,0;0,3;;", "2,0;0,3,;"}){
  bool rejected=false;try{parseNavigationMap(text);}catch(...){rejected=true;}check(rejected);
 }
 check(glfwInit()!=0);glfwWindowHint(GLFW_VISIBLE,GLFW_FALSE);
 auto* window=glfwCreateWindow(64,64,"Map test",nullptr,nullptr);check(window!=nullptr);
 glfwMakeContextCurrent(window);check(glewInit()==GLEW_OK);ImGui::CreateContext();
 auto* world=new World(nullptr);auto* floor=new Entity("floor");floor->addTransformComponent();
 auto* mesh=new Fyredon::Mesh_OBJ("floor","../Assets/Mesh/plane5x5.obj");
 mesh->load_obj();mesh->set_shader("../Shaders/mesh.vert","../Shaders/mesh.frag");
 mesh->load_texture("../Assets/Images/Terrain/RPG_grass_pixel.png",0);
 floor->addComponent(new mesh_component(mesh));
 auto* nav=new navigation_component(floor);floor->addComponent(nav);world->addEntity(floor);
 auto* human=new fbx_entity("human","../Assets/Mesh/MC.fbx");
 human->init("../Shaders/human.vert","../Shaders/human.frag",0,0,0,glm::vec3(0),"../Assets/Images/human/MC.png");world->addEntity(human);
 auto* goal=new Entity("goal");goal->addTransformComponent();world->addEntity(goal);
 nav->character=human;nav->destination=goal;int reloads=0;
 nav->beforeMapReload=[&](){++reloads;world->reset();};
  nav->grid_width=1;check(nav->loadMap());
 // Use an independent fixture: the user's editable map may change between runs.
 std::string fixture;
 for(int z=0;z<11;++z){for(int x=0;x<11;++x){
   if(x)fixture+=",";
   fixture+=char('0'+((x==0||z==0||x==10||z==10)?1:(x==1&&z==1)?2:(x==9&&z==9)?3:0));
 }fixture+=";\n";}
 write(fixture);check(nav->loadMap("map-test.txt"));check(nav->widthCount==11);
 auto* tr=static_cast<transform_component*>(human->getComponentByName("Transform"));
 auto* gt=static_cast<transform_component*>(goal->getComponentByName("Transform"));
 check(tr->matrixTranslation[0]==-4&&tr->matrixTranslation[2]==-4);
 check(gt->matrixTranslation[0]==4&&gt->matrixTranslation[2]==4);
 check(!nav->findPath({-4,-4},{4,4}).empty());
 check(nav->findPath({-5,-5},{4,4}).empty()); // Wall cell
 check(nav->findPath({-100,0},{4,4}).empty()); // Outside map
 auto* old=nav->map;int count=reloads;
 write("2,1;0,0");check(!nav->loadMap("map-test.txt"));check(nav->map==old&&reloads==count);
 check(!nav->loadMap("missing-map-test.txt"));check(nav->map==old);
 for(int i=0;i<30;++i){
  write("2,0,0;0,1,0;0,0,3;");nav->grid_width=0.5f;check(nav->loadMap("map-test.txt"));
  check(nav->walls.size()==1&&nav->width_grid==1.5f&&nav->min_X_grid==-0.75f);
  check(tr->matrixTranslation[0]==-0.5f&&gt->matrixTranslation[2]==0.5f);
  check(!nav->findPath({-0.5f,-0.5f},{0.5f,0.5f}).empty());
  world->init();tr->matrixTranslation[0]=20;world->reset();check(tr->matrixTranslation[0]==-0.5f);
  human->FBXmesh->playCurrentFrame(15);
  write("2,0;0,3");nav->grid_width=2;check(nav->loadMap("map-test.txt"));
  check(nav->walls.empty()&&nav->width_grid==4&&tr->matrixTranslation[0]==-1&&gt->matrixTranslation[2]==1);
  auto* ft=static_cast<transform_component*>(floor->getComponentByName("Transform"));
  check(std::abs(ft->matrixScale[0]-0.4f)<1e-6f);
 }
 auto* movement=new test_move_component();human->addComponent(movement);
 NavigationSystem navigation;navigation.map=floor;navigation.destPoint=goal;navigation.addEntity(human);
 auto* debugEntity=new navigation_entity(&navigation);
 auto* debug=dynamic_cast<navigation_debug_component*>(debugEntity->getComponentByName("Debug"));
 check(debug&&!debug->isActive);
 nav->findPath({-1,-1},{1,1});debug->tick();check(!debug->visible());
 debug->isActive=true;debug->tick();check(debug->visible()&&debug->dashCount()>0);
 int initialDashes=debug->dashCount();
 nav->path.clear();debug->tick();check(debug->dashCount()==initialDashes);
 for(int i=0;i<20;++i){
   debug->isActive=false;debug->tick();check(!debug->visible());
   debug->isActive=true;debug->tick();check(debug->visible());
 }
 nav->findPath({-1,-1},{-1,1});debug->tick();check(debug->visible()&&debug->dashCount()<initialDashes);
 nav->reset();debug->tick();check(!debug->visible());
 auto* animation=static_cast<fbx_property_component*>(human->getComponentByName("Animation"));
 auto step=[&](){
    navigation.lastTime=clock()-CLOCKS_PER_SEC;
    navigation.tick();world->play();
    glfwSetTime(glfwGetTime()+0.05);animation->tick();
 };
 world->play();check(!animation->isPlaying());
 bool walked=false;
 for(int i=0;i<500;++i){step();walked=walked||animation->isPlaying();}
 check(walked&&!animation->isPlaying());
 check(std::abs(tr->matrixTranslation[0]-gt->matrixTranslation[0])<0.0001f);
 check(std::abs(tr->matrixTranslation[2]-gt->matrixTranslation[2])<0.0001f);
 int stoppedFrame=animation->currentFrameIndex;
 for(int i=0;i<10;++i)step();
 check(!animation->isPlaying()&&animation->currentFrameIndex==stoppedFrame);
 gt->matrixTranslation[0]=-1;gt->matrixTranslation[2]=-1;
 for(int i=0;i<10;++i)step();
 check(animation->isPlaying());
 world->stop();check(!animation->isPlaying());
 movement->speed=0;
 for(int i=0;i<10;++i)step();
 check(!animation->isPlaying());movement->speed=1.5f;
 write("2,0,0;1,1,1;0,0,3;");check(nav->loadMap("map-test.txt"));
 for(int i=0;i<10;++i)step();
 check(!animation->isPlaying());
 check(tr->matrixTranslation[0]==-2&&tr->matrixTranslation[2]==-2);
  // Wall selection is applied only on reload, and very large IDs must not overflow.
 write("2,1,0;0,1,0;0,0,3;");
 strcpy_s(nav->wallModelId,"100000000000000000000000000000000000000000000000000000000000000000000000000000001");
 nav->randomWallRotation=false;
 check(nav->loadMap("map-test.txt"));check(nav->wallModelNames.size()==4);
 for(auto id:nav->selectedWallModels)check(id==1);
 for(auto turn:nav->selectedWallTurns)check(turn==0);
 auto oldIds=nav->selectedWallModels;
 strcpy_s(nav->wallModelId,"2");nav->wallOffset={1,0.2f,-1};nav->wallScale={2,1,0.5f};
 nav->tick();check(nav->selectedWallModels==oldIds);
 check(nav->loadMap("map-test.txt"));
 for(auto id:nav->selectedWallModels)check(id==2);
 auto* previousMap=nav->map;
 strcpy_s(nav->wallModelId,"1.5");check(!nav->loadMap("map-test.txt"));check(nav->map==previousMap);
 strcpy_s(nav->wallModelId,"-1");nav->randomWallRotation=true;
 check(nav->loadMap("map-test.txt"));
 for(auto id:nav->selectedWallModels)check(id<4);
 for(auto turn:nav->selectedWallTurns)check(turn>=0&&turn<=3);
 nav->wallOffset=glm::vec3(0);nav->wallScale=glm::vec3(1);
 std::cout<<"PASS: rock directory, huge modulo ID, random selection/rotation and reload-only settings."<<std::endl; // Cell-entry steering intentionally differs from the raw right-angle A* route.
 for(float cell:{1.f,3.f})for(float speed:{1.5f,100.f}){
  write("2,0,0;1,1,0;3,0,0;");nav->grid_width=cell;
  check(nav->loadMap("map-test.txt"));movement->speed=speed;
  bool diagonal=false,arrived=false;
  for(int i=0;i<3000;++i){
   glm::vec2 before(tr->matrixTranslation[0],tr->matrixTranslation[2]);
   step();
   glm::vec2 after(tr->matrixTranslation[0],tr->matrixTranslation[2]);
   check(std::isfinite(after.x)&&std::isfinite(after.y));
   diagonal=diagonal||(std::abs(after.x-before.x)>0.00001f&&std::abs(after.y-before.y)>0.00001f);
   if(glm::length(after-glm::vec2(-cell,cell))<0.0001f){arrived=true;break;}
  }
  check(arrived&&!animation->isPlaying());
  if(speed==1.5f)check(diagonal);
  for(size_t i=1;i<nav->debugPath.size();++i){
   auto d=nav->debugPath[i]-nav->debugPath[i-1];
   check(std::abs(d.x)<0.00001f||std::abs(d.y)<0.00001f);
  }
 }
 std::cout<<"PASS: cell-entry steering, arrival and unchanged raw A* debug route."<<std::endl; // Runtime OBJ/FBX swaps preserve resources and safely rebind animation.
 auto* modelComponent=static_cast<mesh_component*>(human->getComponentByName("Mesh"));
 auto* oldSource=modelComponent->mesh;
 auto* oldTexture=oldSource->tex;
 check(!modelComponent->loadModel("missing-model.obj"));check(modelComponent->mesh==oldSource);
 check(!modelComponent->loadModel("../Assets/script/map.txt"));check(modelComponent->mesh==oldSource);
 for(int swap=0;swap<3;++swap){
   check(modelComponent->loadModel("Assets/Mesh/treasure_box.obj"));
   check(!human->FBXmesh&&modelComponent->mesh->tex==oldTexture);
   check(glIsBuffer(modelComponent->mesh->VBO));
   animation->tick();
   const auto absolute=std::filesystem::absolute("../Assets/Mesh/MC.fbx").u8string();
   check(modelComponent->loadModel(absolute));
   check(human->FBXmesh&&human->FBXmesh==modelComponent->mesh&&modelComponent->mesh->tex==oldTexture);
   human->FBXmesh->playCurrentFrame(15);animation->reset();
   check(glIsBuffer(modelComponent->mesh->VBO));
 }
 auto* goalMesh=new Fyredon::Mesh_OBJ("goalModel","../Assets/Mesh/treasure_box.obj");
 goalMesh->load_obj();goalMesh->set_shader("../Shaders/mesh.vert","../Shaders/mesh.frag");
 goalMesh->load_texture("../Assets/Images/Terrain/treasure_box.png",0);
 auto* goalComponent=new mesh_component(goalMesh);goal->addComponent(goalComponent);
 gt->matrixTranslation[1]=-0.6f;gt->matrixScale[0]=gt->matrixScale[1]=gt->matrixScale[2]=0.4f;
 check(goalComponent->loadModel("Assets/Mesh/MC.fbx"));
 check(dynamic_cast<fbx_property_component*>(goal->getComponentByName("Animation"))!=nullptr);
 goal->tick();check(goalComponent->loadModel("Assets/Mesh/treasure_box.obj"));goal->tick();
 check(gt->matrixTranslation[1]==-0.6f&&gt->matrixScale[0]==0.4f);
 check(nav->loadMap("map-test.txt"));
 check(gt->matrixTranslation[1]==-0.6f&&gt->matrixScale[0]==0.4f);
 std::cout<<"PASS: runtime OBJ/FBX replacement, Unicode path, failed load retention, texture preservation, animation rebinding and target transform."<<std::endl; std::cout<<"PASS: map reload/reset and navigation animation: idle, walking, arrival, stationary frames, retarget, pause, zero speed and unreachable goal; Debug entity toggle, dashes, retained path, repath and reset."<<std::endl;
 return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<std::endl;return 1;}}
