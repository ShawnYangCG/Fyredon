// Copyright Fyredon. All Rights Reserved.
/*=============================================================================
	Terrain_Component.h: ECS Camera Component : Niall, Isiah
=============================================================================*/
#include "Fyredon/Other/pch.h"
// Implements
#include "Fyredon/ECS/terrain_component.h"

#include "Fyredon/ECS/World.h"
#include "Fyredon/Render/Material.h"

// Info : Initalize Terrain Component with default state (to later be modified by user).

Fyredon::terrain_component::terrain_component(Entity* parent)
	: Component("Terrain", parent)
{
	// Default Component Name
	componentName = "Terrain_" + std::to_string(World::get_world().counts.terrain_count);

	// Setup default component parameters
	parms.height_filepath = "..//Assets//Images//Heightmaps//Heightmap.png";
	parms.texture_filepath = "..//Assets//Images//Terrain//grass.jpg";
	parms.res_sqr = 257; 

	// Create Terrain Generator class within inital state
	terrain_generator = new TerrainGenerator(componentName.c_str(), parms.height_filepath);
	terrain_generator->read_height_map(parms.height_filepath);
	terrain_generator->generate_mesh(parms.res_sqr * 2);

	// Override default material paramters
	terrain_generator->TerrainMesh->material->set_tex_diffuse(parms.texture_filepath);
	terrain_generator->TerrainMesh->material->parms.coeff_specular = 0.f;

	// Register Terrain Mesh With Renderer
	Fyredon::Renderer::get_renderer().submit_mesh(terrain_generator->TerrainMesh);
}

Fyredon::terrain_component::~terrain_component()
{
	delete terrain_generator;
}

void Fyredon::terrain_component::tick()
{
	// ...
	Component::tick();
}

void Fyredon::terrain_component::GUI_struct() 
{
	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(250, 200, 150, 255));
	ImGui::Text("Terrain Controls");
	ImGui::PopStyleColor();

	ImGui::Text(parms.height_filepath);
	ImGui::Text(parms.texture_filepath);

	// Ability to change file paths...

	// If res_change, rebuild terrain gen...
}
