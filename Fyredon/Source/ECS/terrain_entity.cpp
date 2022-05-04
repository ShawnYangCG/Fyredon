#include "Fyredon/Other/pch.h"
// Implements
#include "Fyredon/ECS/terrain_entity.h"
#include "Fyredon/Render/Mesh.h"

// Note : This class does not correctly implement Component, we need to fix this...

terrain_entity::terrain_entity()
{
	this->terrain_generator = new TerrainGenerator("heightmap1", "./assets/heightmap/height_map.png");
	terrain_generator->read_height_map("height_map.png");
	terrain_generator->generate_mesh(1081);
	terrain_generator->TerrainMesh->use_tex = false;
	terrain_generator->TerrainMesh->set_shader("..//Shaders//terrain.vert", "..//Shaders//terrain.frag");
	terrain_generator->TerrainMesh->set_colour(glm::vec3(1.f, 0.f, 0.f));
	terrain_generator->TerrainMesh->translate(glm::vec3(0.f, 0.f, 0.f));
	terrain_generator->TerrainMesh->scale(glm::vec3(1.0f));
	terrain_generator->TerrainMesh->mode = Fyredon::Render_Mode::RENDER_MESH;
	terrain_generator->TerrainMesh->name = "terrain";

	std::filesystem::path texture = std::filesystem::current_path() / "..\\Assets\\Images\\Terrain\\input_000_random_sample14.png";
	terrain_generator->TerrainMesh->load_texture(texture.string().c_str(), 0);

	std::cout << terrain_generator->TerrainMesh->flags.texture_set << " text\n";

	this->addTransformComponent();
	Component* terrainMesh = new mesh_component(terrain_generator->TerrainMesh);
	this->addComponent(terrainMesh);

	/*std::cout << "loading tree\n";
	terrain_generator->load_terrain_tree("tree1.obj");
	Component* tree_mesh = new mesh_component(&terrain_generator->tree1);
	addComponent(tree_mesh);*/


}
