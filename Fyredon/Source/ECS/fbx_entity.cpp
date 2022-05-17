#include "Fyredon/Other/pch.h"
#include "Fyredon/ECS/fbx_entity.h"

fbx_entity::fbx_entity(std::string name, std::string filePath)
{
	this->setName(name);
	this-> FBXmesh = new fbx_mesh(name.data(), filePath.data());
}

void fbx_entity::init(const char* shader_vert, const char* shader_frag, float startTime, float endTime, float timeStep)
{
	FBXmesh->load_obj();
	FBXmesh->set_shader(shader_vert, shader_frag);
	std::filesystem::path texture = std::filesystem::current_path() / "..\\Assets\\Images\\human\\rgbTest.png";
	FBXmesh->load_texture(texture.string().c_str(), 0);
	FBXmesh->translate(glm::vec3(0.f, 0.f, 0.f));
	FBXmesh->rotate(0.0f, glm::vec3(1.f, 0.f, 0.f));
	FBXmesh->scale(glm::vec3(1.f, 1.f, 1.f));
	FBXmesh->mode = Fyredon::Render_Mode::RENDER_MESH;

	this->addTransformComponent();

	Component* MESHcomponent = new mesh_component(FBXmesh);
	this->addComponent(MESHcomponent);

	Component* property = new fbx_property_component(startTime, endTime, timeStep);
	this->addComponent(property);
}
