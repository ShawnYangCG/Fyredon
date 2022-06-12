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
	fbx_entity* fbxObject = new fbx_entity("human_walk", "..//Assets//Mesh//walking_2_1.fbx");

	fbxObject->init("..//Shaders//human.vert", "..//Shaders//human.frag", 0.0, 48.0, 3.0);
	
	this->world->addEntity(fbxObject);
}

void Fyredon::MenuAction::createCamera()
{
	camera_entity* entity = new camera_entity();
	this->world->addCamera(entity);
}

void Fyredon::MenuAction::createNavigationDemo()
{
	navigation_entity* navigationSystem = new navigation_entity();
	this->world->addEntity(navigationSystem);

	// add doll
	fbx_entity* fbxObject = new fbx_entity("human_walk", "..//Assets//Mesh//walking_2_1.fbx");
	fbxObject->init("..//Shaders//human.vert", "..//Shaders//human.frag", 0.0, 48.0, 3.0);
	Component* navigation = new test_move_component();
	fbxObject->addComponent(navigation);
	this->world->addEntity(fbxObject);
	navigationSystem->addMoveObject(fbxObject);
	//this->ECS_world->addEntity(fbxObject1);
	//this->aniSys->addFbxEntity(fbxObject1);
	//this->navSys->addEntity(fbxObject1);

	// add target
	Mesh_OBJ* mesh = new Mesh_OBJ("target", "..//Assets//Mesh//target.obj");
	mesh->load_obj();
	mesh->set_shader("..//Shaders//mesh.vert", "..//Shaders//mesh.frag");
	mesh->translate(glm::vec3(0.0f, 0.8f, -0.1f));
	mesh->set_colour(glm::vec3(1.f, 0.f, 0.f));
	mesh->scale(glm::vec3(1.f));
	mesh->mode = Render_Mode::RENDER_MESH;

	Entity* target = new Entity("target");
	target->addTransformComponent();
	mesh_component* meshCom1 = new mesh_component(mesh);
	target->addComponent(meshCom1);
	this->world->addEntity(target);
	navigationSystem->setDest(target);

	// add plane
	Mesh_OBJ* plane5x5 = new Mesh_OBJ("plane5x5", "..//Assets//Mesh//plane5x5.obj");
	plane5x5->load_obj();
	plane5x5->set_shader("..//Shaders//mesh.vert", "..//Shaders//mesh.frag");
	plane5x5->set_colour(glm::vec3(0.2f, 0.2f, 0.2f));
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
}

void Fyredon::MenuAction::render_settings()
{
	if (panelOrgnizer->gui_panels.find("render_settings") == panelOrgnizer->gui_panels.end()) {
		panelOrgnizer->gui_panels["render_settings"] = new GUI_RendererSettings;
	}
	else delete panelOrgnizer->gui_panels["render_settings"];
}