#include "Fyredon/Other/pch.h"
#include "Fyredon/Action/MenuAction.h"

Fyredon::MenuAction::MenuAction(GUI_PanelOrgnizer* panelOrgnizer, World* world)
{
	this->world = world;
	this->panelOrgnizer = panelOrgnizer;
	this->menuPanel = (GUI_Menu*)panelOrgnizer->gui_panels["Menu"];
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
	if (menuPanel->menu_state.createDoll) {
		createDoll();
		menuPanel->menu_state.createDoll = false;
	}
	if (menuPanel->menu_state.createCamera) {
		createCamera();
		menuPanel->menu_state.createCamera = false;
	}
	if (menuPanel->menu_state.createTest) {
		createTest();
		menuPanel->menu_state.createTest = false;
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
	Mesh* mesh = new Mesh("test_pig", "..//Assets//Mesh//pighead.obj");
	mesh->load_obj(false);
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

void Fyredon::MenuAction::createDoll()
{
	fbx_entity* fbxObject1 = new fbx_entity("human_walk", "..//Assets//Mesh//walking_2.fbx", 0.0, 1.0, 3.0);

	//Mesh* mesh = ((mesh_component*)fbxObject1->getComponentByName("Mesh"))->mesh;
	fbxObject1->FBXmesh->set_shader("..//Shaders//mesh.vert", "..//Shaders//mesh.frag");
	fbxObject1->FBXmesh->set_colour(glm::vec3(0.f, 1.f, 0.f));
	fbxObject1->FBXmesh->translate(glm::vec3(0.f, 0.f, 0.f));
	fbxObject1->FBXmesh->rotate(0.0f, glm::vec3(1.f, 0.f, 0.f));
	fbxObject1->FBXmesh->scale(glm::vec3(1.f, 1.f, 1.f));
	fbxObject1->FBXmesh->mode = Render_Mode::RENDER_MESH;

	fbxObject1->addTransformComponent();
	mesh_component* MESHcomponent = new mesh_component(fbxObject1->FBXmesh);
	fbxObject1->addComponent(MESHcomponent);
	Component* property = new fbx_property_component(0.0, 1.0, 3.0);
	fbxObject1->addComponent(property);
	//Component* navigation = new test_move_component();
	//fbxObject1->addComponent(navigation);
	this->world->addEntity(fbxObject1);
	this->world->aniSys->addFbxEntity(fbxObject1);
}

void Fyredon::MenuAction::createCamera()
{
	camera_entity* entity = new camera_entity();
	this->world->addCamera(entity);
}

void Fyredon::MenuAction::createTest()
{
	Entity* target = new Entity("target");
	Mesh* mesh3 = new Mesh("target", "..//Assets//Mesh//target.obj");
	mesh3->load_obj(false);
	mesh3->set_shader("..//Shaders//mesh.vert", "..//Shaders//mesh.frag");
	mesh3->set_colour(glm::vec3(1.f, 0.f, 0.f));
	mesh3->translate(glm::vec3(0.0f, 0.8f, -0.1f));
	mesh3->mode = Render_Mode::RENDER_MESH;
	target->addTransformComponent();
	mesh_component* mshCop1 = new mesh_component(mesh3);
	target->addComponent(mshCop1);
	this->world->addEntity(target);
	this->world->navSys->destPoint = target;

	Entity* plane = new Entity("plane5x5");
	Mesh* mesh2 = new Mesh("plane5x5", "..//Assets//Mesh//plane5x5.obj");
	mesh2->load_obj(false);
	mesh2->set_shader("..//Shaders//mesh.vert", "..//Shaders//mesh.frag");
	mesh2->set_colour(glm::vec3(0.2f, 0.2f, 0.2f));
	mesh2->mode = Render_Mode::RENDER_MESH;
	plane->addTransformComponent();
	mesh_component* mshCop = new mesh_component(mesh2);
	plane->addComponent(mshCop);
	//navigation_component* navCop = new navigation_component(plane);
	//plane->addComponent(navCop);
	this->world->addEntity(plane);
	this->world->navSys->map = plane;
}
