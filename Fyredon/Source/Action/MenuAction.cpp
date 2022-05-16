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
	//fbx_mesh* mesh = (fbx_mesh*)((mesh_component*)fbxObject->getComponentByName("Mesh"))->mesh;
	//std::filesystem::path texture = std::filesystem::current_path() / "..\\Assets\\Images\\Terrain\\input_000_random_sample142.png";
	//mesh->load_texture(texture.string().c_str(), 0);
	fbxObject->init("..//Shaders//human.vert", "..//Shaders//human.frag", 0.0, 48.0, 3.0);
	
	
	//Component* navigation = new test_move_component();
	//fbxObject1->addComponent(navigation);
	this->world->addEntity(fbxObject);
	//this->aniSys->addFbxEntity(fbxObject1);
}

void Fyredon::MenuAction::createCamera()
{
	camera_entity* entity = new camera_entity();
	this->world->addCamera(entity);
}

void Fyredon::MenuAction::render_settings()
{
	if (panelOrgnizer->gui_panels.find("render_settings") == panelOrgnizer->gui_panels.end()) {
		panelOrgnizer->gui_panels["render_settings"] = new GUI_RendererSettings;
	}
	else delete panelOrgnizer->gui_panels["render_settings"];
}