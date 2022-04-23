#include "Fyredon/Other/pch.h"
#include "Fyredon/ECS/terrain_entity.h"

terrain_entity::terrain_entity()
{
	this->terrain_generator = new TerrainGenerator("heightmap1", "./assets/heightmap/Heightmap.png");
	terrain_generator->read_height_map("Heightmap.png");
	terrain_generator->generate_mesh(257 * 2);
	terrain_generator->TerrainMesh.use_tex = false;
	terrain_generator->TerrainMesh.create_buffers();
	terrain_generator->TerrainMesh.set_shader("..//Shaders//terrain.vert", "..//Shaders//terrain.frag");
	terrain_generator->TerrainMesh.set_colour(glm::vec3(1.f, 0.f, 0.f));
	terrain_generator->TerrainMesh.translate(glm::vec3(0.f, 0.f, 0.f));
	terrain_generator->TerrainMesh.scale(glm::vec3(1.0f));
	terrain_generator->TerrainMesh.mode = Fyredon::Render_Mode::RENDER_MESH;
	terrain_generator->TerrainMesh.name = "terrain";

	this->addTransformComponent();
	Component* terrainMesh = new mesh_component(&terrain_generator->TerrainMesh);
	this->addComponent(terrainMesh);
}
