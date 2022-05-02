// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	GameWorld.h: Temporary (Pre-ECS) Container to whole state of data in game : Niall
=============================================================================*/
#include "Fyredon/Other/pch.h"
// Implements
#include "Fyredon/Core/GameWorld.h"

#include "Fyredon/Render/Renderer.h"
#include "Fyredon/Render/Mesh.h"

#include "Fyredon/Debug/Logger.h"
#include "Fyredon/Core/RandomGenerator.h"

#include "GLEW/glew.h"

Fyredon::GameWorld Fyredon::GameWorld::world; 

Fyredon::GameWorld& Fyredon::GameWorld::get_world()
{
	return world; 
}

Fyredon::GameWorld::GameWorld() : total_game_time(0.f) {
}

// Info : Tick each entity in game world
void Fyredon::GameWorld::tick(float delta_time)
{
//	for (Primitive& prim : prims) // Update Transforms ...
//  for (Light& light : lights)   // Update Transforms

	// Tick Entities... (we have nothing tickable currently!)

	total_game_time += delta_time;
}

// Info : Current (shabby) creation of a world entity :   
// 1. Allocate to internal array. 
// 2. Define mapping for named access.
// 3. Submit pointer to Renderer (draw list). 
// ToDo : also pass per prim xform + render commands

// Info : Handles creation of meshes, this format should be extended for creation of other game entitiy/components.
void Fyredon::GameWorld::create_mesh_obj(const char* name, const char* file_path)
{
	auto rng = Fyredon::RandomGenerator(1234);
	float x = rng.getDouble();
	rng = Fyredon::RandomGenerator(1324);
	float y = rng.getDouble();
	rng = Fyredon::RandomGenerator(4321);
	float z = rng.getDouble();

	Mesh_OBJ mesh(name, file_path);
	mesh.load_obj();
	mesh.set_shader("..//Shaders//mesh.vert", "..//Shaders//mesh.frag");
	mesh.translate(glm::vec3(x, y, z));

	// Setup
	prims.push_back(std::move(mesh));
	Mesh* mesh_p = &prims.back();
	prims_map[std::string(name)] = mesh_p;

	// Submit to renderer
	Renderer::get_renderer().submit_mesh(mesh_p);
}

/*
void Fyredon::GameWorld::create_mesh_obj(const char* name, const char* file_path, const char* vert_shader, const char* frag_shader)
{
	Mesh mesh(name, file_path);
	prims.push_back(std::move(mesh));
	Primitive* mesh_p = &prims.back();
	prims_map[std::string(name)] = mesh_p;

	// Set Shader
	mesh_p->set_shader(vert_shader, frag_shader);

	// Call load
	static_cast<Mesh*>(mesh_p)->load_obj();

	// Submit to renderer
	Renderer::get_renderer().submit_primitive(mesh_p);
} */

void Fyredon::GameWorld::create_light(const char* name, glm::vec3 Pos, float Strength)
{
	lights.emplace_back(Pos, Strength);
	Light* light = &lights.back();
	lights_map[std::string(name)] = light; 

	Renderer::get_renderer().submit_light(light);
}

void Fyredon::GameWorld::delete_mesh(const char* name)
{
	auto found = prims_map.find(std::string(name));
	if (found == prims_map.end())
	{
		FD_DEBUG_BLOCK_MSG("Tried to delete non existing mesh: ", "s", name);
		return; 
	}
	else
	{
		std::size_t idx = found->second - prims.data();
		prims.erase(prims.begin() + idx);
		prims_map.erase(std::string(name));
	}

	// ToDo, delete from renderer draw list
}

void Fyredon::GameWorld::delete_light(const char* name)
{
	auto found = lights_map.find(std::string(name));
	if (found == lights_map.end())
	{
		FD_DEBUG_BLOCK_MSG("Tried to delete non existing light: ", "s", name);
		return;
	}
	else
	{
		std::size_t idx = found->second - lights.data();
		prims.erase(prims.begin() + idx);
		lights_map.erase(std::string(name));
	}

	// ToDo, delete from renderer draw list
}

void Fyredon::GameWorld::create_terrain() {
	terrain_generator = new TerrainGenerator("heightmap1", "./assets/heightmap/Heightmap.png");
	terrain_generator->read_height_map("Heightmap.png");
	terrain_generator->generate_mesh(257*2);
	
	//terrain_generator->TerrainMesh->use_tex = true; // Disable Tex
	terrain_generator->TerrainMesh->set_shader("..//Shaders//mesh.vert", "..//Shaders//mesh.frag");
	terrain_generator->TerrainMesh->set_colour(glm::vec3(1.f, 0.f, 0.f));
	terrain_generator->TerrainMesh->translate(glm::vec3(0.f, 0.f, 0.f));
	terrain_generator->TerrainMesh->scale(glm::vec3(1.0f));
	terrain_generator->TerrainMesh->mode = Render_Mode::RENDER_MESH;
	terrain_generator->TerrainMesh->name = "terrain";

	// Ownership issue as TerrainMesh exists within terrain_generator...
	prims.push_back(*(terrain_generator->TerrainMesh));
	Mesh* mesh_p = &prims.back();
	prims_map[std::string("terrain")] = mesh_p;

	// Submit to renderer
	Renderer::get_renderer().submit_mesh(mesh_p);


	//TerrainGenerator* terrain_generator2 = new TerrainGenerator("heightmap12", "./assets/heightmap/Heightmap.png");
	//terrain_generator2->read_height_map("Heightmap.png");
	//terrain_generator2->generate_mesh(257 );
	//terrain_generator2->TerrainMesh.use_tex = false;
	//terrain_generator2->TerrainMesh.create_buffers();
	//terrain_generator2->TerrainMesh.set_shader("..//Shaders//mesh.vert", "..//Shaders//mesh.frag");
	//terrain_generator2->TerrainMesh.set_colour(glm::vec3(1.f, 1.f, 0.f));
	//terrain_generator2->TerrainMesh.translate(glm::vec3(0.f, 0.f, 0.f));
	//terrain_generator2->TerrainMesh.scale(glm::vec3(0.02f));
	//terrain_generator2->TerrainMesh.mode = Render_Mode::RENDER_MESH;

	//terrain_generator2->TerrainMesh.name = "terrainn";
	//prims.push_back(std::move(terrain_generator2->TerrainMesh));
	//Primitive* mesh_p2 = &prims.back();
	//prims_map[std::string("terrainn")] = mesh_p2;
	//Renderer::get_renderer().submit_primitive(mesh_p2);

	//terrain_generator->draw_water_quad();
	//terrain_generator->water_quad.use_tex = false;
	//terrain_generator->water_quad.create_buffers();
	//terrain_generator->water_quad.set_shader("..//Shaders//mesh.vert", "..//Shaders//mesh.frag");
	//terrain_generator->water_quad.set_colour(glm::vec3(0.f, 0.f, 1.f));
	//terrain_generator->water_quad.translate(glm::vec3(0.f, 0.f, 0.f));
	//terrain_generator->water_quad.scale(glm::vec3(0.02f));
	//terrain_generator->water_quad.mode = Render_Mode::RENDER_MESH;

	//terrain_generator->water_quad.name = "water";
	//prims.push_back(std::move(terrain_generator->water_quad));
	//Primitive*  mesh_water = &prims.back();
	//prims_map[std::string("water")] = mesh_water;
	//Renderer::get_renderer().submit_primitive(mesh_water);

	terrain_generator->terrain_loaded = true;
}