#include "Fyredon/Other/pch.h"
// Implements
#include "Fyredon/Action/MenuAction.h"

#include "Fyredon/Render/Mesh_OBJ.h"
#include "Fyredon/GUI/GUI_RendererSettings.h"

Fyredon::MenuAction::MenuAction(GUI_PanelOrgnizer* panelOrgnizer, World* world)
{
	this->world = world;
	this->panelOrgnizer = panelOrgnizer;
	this->menuPanel = (GUI_Menu*)panelOrgnizer->gui_panels["Menu"];
}

Fyredon::MenuAction::~MenuAction()
{
	if(this->world)free(this->world);
	if (this->panelOrgnizer)free(this->panelOrgnizer);
	if (this->menuPanel)free(this->menuPanel);
}

void Fyredon::MenuAction::tick()
{
	if (menuPanel->menu_state.IsNew) {
		menuPanel->new_project();
	}
	if (menuPanel->menu_state.save) {
		menuPanel->menu_state.save = false;
	}
	if (menuPanel->menu_state.createTerrain) {
		createTerrainEntity();
		menuPanel->menu_state.createTerrain = false;
	}
	if (menuPanel->menu_state.pigHead) {
		createPigHead();
		menuPanel->menu_state.pigHead = false;
	}
	if (menuPanel->menu_state.tree1) {
		createTree1();
		menuPanel->menu_state.tree1 = false;
	}
	if (menuPanel->menu_state.createDoll) {
		createDoll();
		menuPanel->menu_state.createDoll = false;
	}
	if (menuPanel->menu_state.createCamera) {
		createCamera();
		menuPanel->menu_state.createCamera = false;
	}
	if (menuPanel->menu_state.createNavigationDemo) {
		createNavigationDemo();
		menuPanel->menu_state.createNavigationDemo = false;
	}
	if (menuPanel->menu_state.createDanceDemo) {
        menuPanel->menu_state.createDanceDemo = false;
        createDanceDemo();
    }
    if (menuPanel->menu_state.view_renderer)
	{
		render_settings();
		menuPanel->menu_state.view_renderer = false; 
	}
}

void Fyredon::MenuAction::createTerrainEntity()
{
	terrain_entity* entity = new terrain_entity();
	entity->setName(entity->getName() + "_terrain");
	this->world->addEntity(entity);
}

void Fyredon::MenuAction::createPigHead()
{
	Mesh_OBJ* mesh = new Mesh_OBJ("test_pig", "..//Assets//Mesh//pighead.obj");
	mesh->load_obj();
	mesh->set_shader("..//Shaders//mesh.vert", "..//Shaders//mesh.frag");
	mesh->set_colour(glm::vec3(1.f, 0.f, 0.f));
	mesh->translate(glm::vec3(0.f, 0.2f, 2.f));
	mesh->scale(glm::vec3(2.f));
	mesh->mode = Render_Mode::RENDER_MESH;

	Entity* entity = new Entity("Pig Head");
	entity->addTransformComponent();
	mesh_component* meshCom = new mesh_component(mesh);
	entity->addComponent(meshCom);
	this->world->addEntity(entity);
}

void Fyredon::MenuAction::createTree1()
{
	Mesh_OBJ* mesh = new Mesh_OBJ("tree1", "..//Assets//Mesh//tree1.obj");
	mesh->load_obj();
	mesh->set_shader("..//Shaders//mesh.vert", "..//Shaders//mesh.frag");
	mesh->set_colour(glm::vec3(1.f, 0.f, 0.f));
	mesh->translate(glm::vec3(0.f, 0.2f, 2.f));
	mesh->scale(glm::vec3(2.f));
	mesh->mode = Render_Mode::RENDER_MESH;
	Entity* entity = new Entity("Tree1");
	entity->addTransformComponent();
	mesh_component* meshCom = new mesh_component(mesh);
	entity->addComponent(meshCom);
	this->world->addEntity(entity);
}

void Fyredon::MenuAction::createDoll()
{
	int countX = 2, countY = 2;
	int startPos_x = -2, startPos_y = -2;
	for (int j = 0 + startPos_y; j < countY + startPos_y; j++) {
		for (int i = 0 + startPos_x; i < countX + startPos_x; i++) {
			fbx_entity* fbxObject = new fbx_entity("human_walk", "..//Assets//Mesh//box2.fbx");

			fbxObject->init("..//Shaders//human.vert", "..//Shaders//human.frag", 0.0, 48.0, 3.0, glm::vec3(i, 0, j));

			this->world->addEntity(fbxObject);
		}
	}
	
}

void Fyredon::MenuAction::createCamera()
{
	camera_entity* entity = new camera_entity();
	this->world->addCamera(entity);
}

void Fyredon::MenuAction::createNavigationDemo()
{
	auto* settings = static_cast<GUI_RendererSettings*>(panelOrgnizer->gui_panels.at("setting"));
	settings->state.draw_grid = false;
	settings->state.draw_axis = false;

	navigation_entity* navigationSystem = new navigation_entity();
	this->world->addEntity(navigationSystem);

	// add doll
	fbx_entity* fbxObject = new fbx_entity("human_walk", "..//Assets//Mesh//MC.fbx");
	fbxObject->init("..//Shaders//human.vert", "..//Shaders//human.frag", 0.0, 48.0, 3.0, glm::vec3(0.f), "../Assets/Images/human/MC.png");
    // MC exports at ~8.6 cm tall; present it at ~1.72 m in this demo.
    auto* characterTransform = static_cast<transform_component*>(fbxObject->getComponentByName("Transform"));
    for (auto& scale : characterTransform->matrixScale) scale = 20.f;

	Component* navigation = new test_move_component();
	fbxObject->addComponent(navigation);
	this->world->addEntity(fbxObject);
	navigationSystem->addMoveObject(fbxObject);
	//this->ECS_world->addEntity(fbxObject1);
	//this->aniSys->addFbxEntity(fbxObject1);
	//this->navSys->addEntity(fbxObject1);

	// add target
	Mesh_OBJ* mesh = new Mesh_OBJ("target", "../Assets/Mesh/treasure_box.obj");
	mesh->load_obj();
	mesh->set_shader("..//Shaders//mesh.vert", "..//Shaders//mesh.frag");
	    // Keep the chest centered on the target cell with its base on the ground.
    if (!mesh->vert_data.empty()) {
        glm::vec3 lo=mesh->vert_data.front().pos, hi=lo;
        for (const auto& vertex:mesh->vert_data) {
            lo=glm::min(lo,vertex.pos);hi=glm::max(hi,vertex.pos);
        }
        mesh->translate(glm::vec3(-(lo.x+hi.x)*0.5f,-lo.y,-(lo.z+hi.z)*0.5f));
    }
    mesh->set_colour(glm::vec3(1.f));
    mesh->load_texture("../Assets/Images/Terrain/treasure_box.png", 0);
	mesh->scale(glm::vec3(1.f));
	mesh->mode = Render_Mode::RENDER_MESH;

	Entity* target = new Entity("target");
	target->addTransformComponent();
    auto* targetTransform=static_cast<transform_component*>(target->getComponentByName("Transform"));
    targetTransform->matrixTranslation[1]=-0.6f;
    for(auto& scale:targetTransform->matrixScale)scale=0.4f;
	mesh_component* meshCom1 = new mesh_component(mesh);
	target->addComponent(meshCom1);
	this->world->addEntity(target);
	navigationSystem->setDest(target);

	// add plane
	Mesh_OBJ* plane5x5 = new Mesh_OBJ("plane5x5", "..//Assets//Mesh//plane5x5.obj");
	plane5x5->load_obj();
	plane5x5->set_shader("..//Shaders//mesh.vert", "..//Shaders//mesh.frag");
	plane5x5->set_colour(glm::vec3(0.2f, 0.2f, 0.2f));
    plane5x5->load_texture("../Assets/Images/Terrain/RPG_grass_pixel.png", 0);
	plane5x5->scale(glm::vec3(1.f));
	plane5x5->mode = Render_Mode::RENDER_MESH;

	Entity* plane = new Entity("plane5x5");
	plane->addTransformComponent();
	mesh_component* meshCom2 = new mesh_component(plane5x5);
	plane->addComponent(meshCom2);
	navigation_component* navCop = new navigation_component(plane);
	plane->addComponent(navCop);
	this->world->addEntity(plane);
	navigationSystem->setMap(plane);
    navCop->character=fbxObject;
    navCop->destination=target;
    navCop->grid_width=1.f;
    navCop->beforeMapReload=[this,navigationSystem]() {
        menuPanel->menu_state.play=false;
        menuPanel->menu_state.pause=false;
        menuPanel->menu_state.reset=false;
        world->reset();
        navigationSystem->navigationSys->lastTime=clock();
    };
    navCop->loadMap();
}

void Fyredon::MenuAction::render_settings()
{
	// Keep the existing settings alive: the renderer holds pointers to their state.
	auto* settings = static_cast<GUI_RendererSettings*>(panelOrgnizer->gui_panels.at("setting"));
	settings->visible = !settings->visible;
}
void Fyredon::MenuAction::createDanceDemo()
{
    auto* settings=static_cast<GUI_RendererSettings*>(panelOrgnizer->gui_panels.at("setting"));
    settings->state.draw_grid=false;
    settings->state.draw_axis=false;

    auto* groundMesh=new Mesh_OBJ("Dance ground","../Assets/Mesh/plane5x5.obj");
    groundMesh->load_obj();
    groundMesh->set_shader("../Shaders/mesh.vert","../Shaders/mesh.frag");
    groundMesh->load_texture("../Assets/Images/Terrain/RPG_grass_pixel.png",0);
    auto* ground=new Entity("Dance ground");
    ground->addTransformComponent();
    ground->addComponent(new mesh_component(groundMesh));
    world->addEntity(ground);

    // Separate entities keep each rock independently editable in the Inspector.
    const glm::vec2 positions[]={{-2.f,-2.f},{2.f,-2.f},{-2.f,2.f},{2.f,2.f}};
    for(int i=0;i<4;++i){
        const auto name="rock"+std::to_string(i+1);
        const auto file="../Assets/Mesh/rocks/"+name+".obj";
        auto* rockMesh=new Mesh_OBJ(name.c_str(),file.c_str());
        rockMesh->load_obj();
        rockMesh->set_shader("../Shaders/mesh.vert","../Shaders/mesh.frag");
        rockMesh->set_colour(glm::vec3(0.55f,0.58f,0.6f));
        auto lo=rockMesh->vert_data.at(0).pos,hi=lo;
        for(const auto& vertex:rockMesh->vert_data){lo=glm::min(lo,vertex.pos);hi=glm::max(hi,vertex.pos);}
        const float scale=1.3f/std::max(0.001f,std::max(hi.x-lo.x,hi.z-lo.z));
        rockMesh->translate(-glm::vec3((lo.x+hi.x)*0.5f,lo.y,(lo.z+hi.z)*0.5f)*scale);
        auto* rock=new Entity(name);
        rock->addTransformComponent();
        auto* tr=static_cast<transform_component*>(rock->getComponentByName("Transform"));
        tr->matrixTranslation[0]=positions[i].x;tr->matrixTranslation[2]=positions[i].y;
        for(auto& value:tr->matrixScale)value=scale;
        rock->addComponent(new mesh_component(rockMesh));
        world->addEntity(rock);
    }

    auto* dancer=new fbx_entity("MC_dance","../Assets/Mesh/MC_dance.fbx");
    dancer->init("../Shaders/human.vert","../Shaders/human.frag",0,0,0,glm::vec3(0),"../Assets/Images/human/MC.png");
    auto lo=dancer->FBXmesh->vert_data.at(0).pos,hi=lo;
    for(const auto& vertex:dancer->FBXmesh->vert_data){lo=glm::min(lo,vertex.pos);hi=glm::max(hi,vertex.pos);}
    const float scale=1.7f/std::max(hi.y-lo.y,0.001f);
    dancer->FBXmesh->translate(-glm::vec3((lo.x+hi.x)*0.5f,lo.y,(lo.z+hi.z)*0.5f)*scale);
    auto* tr=static_cast<transform_component*>(dancer->getComponentByName("Transform"));
    for(auto& value:tr->matrixScale)value=scale;
    world->addEntity(dancer);
}