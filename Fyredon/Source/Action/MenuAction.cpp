// Copyright Fyredon. All Rights Reserved.
/*=============================================================================
	Menu_Action.cpp: Creates Entites in World based on Menu Actions: Shawn, Niall
=============================================================================*/
#include "Fyredon/Other/pch.h"
// Implements
#include "Fyredon/Action/MenuAction.h"

// Project Includes 
#include "Fyredon/Render/Mesh_OBJ.h"
#include "Fyredon/GUI/GUI_RendererSettings.h"
#include "Fyredon/ECS/World.h"

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

	if (menuPanel->menu_state.view_renderer)
	{
		render_settings();
		menuPanel->menu_state.view_renderer = false; 
	}
}

// Info : Creates Camera Entity where Camera_Component is the root component. 

void Fyredon::MenuAction::createCamera()
{
	World::get_world().create_camera_entity();
}

void Fyredon::MenuAction::createTerrainEntity()
{
	//terrain_entity* entity = new terrain_entity();
	//entity->setName(entity->getName() + "_terrain");
	//this->world->addEntity(entity);
}

void Fyredon::MenuAction::createPigHead()
{
	// Rewrite to create doll using Entity and Components only...
	/*
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
	//this->world->addEntity(entity); */
}

void Fyredon::MenuAction::createDoll()
{
	// Rewrite to create doll using Entity and Components only...
	/*
	fbx_entity* fbxObject1 = new fbx_entity("human_walk", "./walking_2.fbx", 0.0, 1.0, 3.0);
	Mesh* mesh = ((mesh_component*)fbxObject1->getComponentByName("Mesh"))->mesh;
	mesh->set_shader("../../shaders/mesh.vert", "../../shaders/mesh.frag");
	mesh->set_colour(glm::vec3(1.f, 0.f, 0.f));
	mesh->translate(glm::vec3(0.f, 0.f, 0.f));
	mesh->rotate(0.0f, glm::vec3(1.f, 0.f, 0.f));
	mesh->scale(glm::vec3(1.f, 1.f, 1.f));
	mesh->mode = Render_Mode::RENDER_MESH;
	Component* navigation = new test_move_component();
	fbxObject1->addComponent(navigation);
	this->world->addEntity(fbxObject1);
	//this->aniSys->addFbxEntity(fbxObject1);
	*/
}

void Fyredon::MenuAction::render_settings()
{
	if (panelOrgnizer->gui_panels.find("render_settings") == panelOrgnizer->gui_panels.end()) {
		panelOrgnizer->gui_panels["render_settings"] = new GUI_RendererSettings;
	}
	else {
		delete panelOrgnizer->gui_panels["render_settings"];
		panelOrgnizer->gui_panels["render_settings"] = new GUI_RendererSettings;
	}
}