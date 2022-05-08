#include "Fyredon/Other/pch.h"
// Implements
#include "Fyredon/ECS/terrain_entity.h"

#include "Fyredon/Render/Mesh.h"
#include "Fyredon/Render/Material.h"


terrain_entity::terrain_entity()
{
	// Allocate Terrain Generator itself
	this->terrain_generator = new TerrainGenerator("heightmap1", "./assets/heightmap/Heightmap.png");
	terrain_generator->read_height_map("Heightmap.png");
	terrain_generator->generate_mesh(257 * 2);

	// Override default Fyredon::Mesh Shader
	terrain_generator->TerrainMesh->set_shader("..//Shaders//mesh_vert_common.glsl", "..//Shaders//mesh-terrain_fragment.glsl");

	// Override default material paramters
	terrain_generator->TerrainMesh->material->set_tex_diffuse("..//Assets//Images//Terrain//grass.jpg");
	terrain_generator->TerrainMesh->material->parms.coeff_specular = 0.f; 
	terrain_generator->TerrainMesh->name = "terrain";

	// Terrain is Root Component
	// Mesh Component from Terrain Generator
	Component* terrainMesh = new mesh_component(terrain_generator->TerrainMesh);
	this->addComponent(terrainMesh);

	// Transform Component atop
	this->addTransformComponent();
}
