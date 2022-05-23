// Copyright Fyredon. All Rights Reserved.
/*=============================================================================
	Mesh_Component.cpp: ECS Mesh Component : Niall
=============================================================================*/
#include "Fyredon/Other/pch.h"
// Implements
#include "Fyredon/ECS/mesh_component.h"

#include "Fyredon/ECS/World.h"
#include "Fyredon/Render/Material.h"

mesh_component::mesh_component(Entity* parent)
{
	bounds.min = glm::vec3(0.f), bounds.max = glm::vec3(1.f);
	ren_idx = 0;

	World& world = World::get_world();
	componentName = std::string("Mesh_") + std::to_string(world.counts.mesh_count++);

	// ======== Allocate Inner MeshOBJ Instance 
	// Default Mesh OBJ (Shaderball)
	mesh = new Fyredon::Mesh_OBJ(componentName.c_str(), "..//Assets//Mesh//shaderball.obj");
	mesh->load_obj();
	// def transforms (will be overriden when changed)
	mesh->scale(glm::vec3(2.f)); 
	//mesh->translate(glm::vec3(0.1f, 0.f, 0.1f));

	// ======== Submit mesh to renderer draw list 
	ren_idx = Fyredon::Renderer::get_renderer().submit_mesh(this->mesh);

	// ======== Init Parms
	parms.file_path = "..//";

	// ======== Default GUI Thumbnail Textures
	parms.thumb_diff = new Fyredon::TextureImage("..//Assets//Images//Icons//unknown-icon.png", "unk", 0, true); parms.thumb_diff->load();
	parms.thumb_spec = new Fyredon::TextureImage("..//Assets//Images//Icons//unknown-icon.png", "unk", 1, true); parms.thumb_spec->load();
	parms.thumb_bump = new Fyredon::TextureImage("..//Assets//Images//Icons//unknown-icon.png", "unk", 1, true); parms.thumb_bump->load();
	parms.thumb_mat  = new Fyredon::TextureImage("..//Assets//Images//Icons//unknown-icon.png", "unk", 2, true); parms.thumb_mat->load();

	// ======== Default Temp Buffers
	strcpy_s(parms.file_buf, 511, "..//Assets//Mesh//shaderball.obj");
	strcpy_s(parms.diff_buf, 511, "..//Assets//Images//Textures//uv_map.png");
	strcpy_s(parms.spec_buf, 511, "..//Assets//Images//Textures//spec_mapb.jpg");
	strcpy_s(parms.bump_buf, 511, "..//Assets//Images//Textures//spec_mapb.jpg");

	// ======== ECS State
	this->parent = parent;
	this->memmory_size = sizeof(mesh_component);
}

// Remove mesh from render_list if component deleted, then delete inner mesh.
mesh_component::mesh_component::~mesh_component()
{
	Fyredon::Renderer::get_renderer().delete_mesh(ren_idx);
	delete mesh; 
}

void mesh_component::mesh_changed()
{
	/*
	std::ifstream test(parms.file_path);
	if (!test.is_open()) {
		std::cerr << "ERROR::Mesh_Component::Invalid Mesh File Path." << std::endl;
		return;
	} */

	if (mesh) {
		Fyredon::Renderer::get_renderer().delete_mesh(ren_idx);
		delete mesh;
	}
	mesh = new Fyredon::Mesh_OBJ(componentName.c_str(), parms.file_path);
	mesh->load_obj();
	ren_idx = Fyredon::Renderer::get_renderer().submit_mesh(this->mesh);
}

void mesh_component::GUI_struct()
{
	static float trans[3] = { 0.f, 0.f, 0.f };

	// Temp GUI vec3s 
	static float col_amb[3] = { mesh->material->parms.colour_ambient.x, mesh->material->parms.colour_ambient.y, mesh->material->parms.colour_ambient.z };
	static float col_dif[3] = { mesh->material->parms.colour_diffuse.x, mesh->material->parms.colour_diffuse.y, mesh->material->parms.colour_diffuse.z };
	static float col_spc[3] = { mesh->material->parms.colour_specular.x, mesh->material->parms.colour_specular.y, mesh->material->parms.colour_specular.z };
	
	// ================ Mesh Properties ================
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(250, 200, 150, 255));
	ImGui::Text("Mesh Properties");
	ImGui::PopStyleColor();
	std::string vertex = "Vertex: " + std::to_string(this->mesh->vert_count);
	std::string face   = "Face: "   + std::to_string(this->mesh->index_count / 3);
	ImGui::Text(vertex.data());
	ImGui::Text(face.data());

	ImGui::InputText("Mesh File", parms.file_buf, 512);
	if (ImGui::Button("Load Mesh")) {
		parms.file_path = parms.file_buf;
		mesh_changed();
	}

	// This is a hack as the Transform Component is broken and I dont have time to fix it..!
	if (ImGui::SliderFloat3("Translate", trans, -5.f, 5.f))
	{
		mesh->model = glm::mat4(1.f);
		mesh->scale(glm::vec3(2.f));
		mesh->translate(glm::vec3(trans[0], std::fmax(trans[1], 0.f), trans[2]));
	}

	// ================ Draw Material GUI ================
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(250, 200, 150, 255));
	ImGui::Text("Material Settings");
	ImGui::PopStyleColor();
	// Material Thumbnail
	//ImGui::Image(parms.thumb_mat->get_imgui_texture(), ImVec2(128, 128), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(1.0f, 1.0f, 1.0f, 0.5f));

	// ================ Material Maps ================
	// ======== Diffuse Map
	ImGui::Separator();
	ImGui::Text("Diffuse Map");
	if (ImGui::Checkbox("Use Diff", &parms.use_diff)) {
		// Update Material
		mesh->material->build_mesh_shader();
	}
	// Diff Map Thumbnail
	ImVec4 diff_border = parms.use_diff ? ImVec4(0.f, 1.f, 0.f, .5f) : ImVec4(1.f, 1.f, 1.f, .5f);
	ImTextureID d_tex = parms.use_diff ? parms.thumb_diff->get_imgui_texture() : 0;
	ImGui::Image(d_tex, ImVec2(64, 64), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), diff_border);

	// Diff Map File
	ImGui::InputText("0", parms.diff_buf, 512);
	ImGui::SameLine();
	if (ImGui::Button("Load Diff Map")) 
	{
		parms.diff_path = parms.diff_buf;
		// Update Thumb
		if (parms.thumb_diff) delete parms.thumb_diff;
		parms.thumb_diff = new Fyredon::TextureImage(parms.diff_buf, "thumb_diff", 1, true); parms.thumb_diff->load();
		// Update Material
		mesh->material->parms.use_diff_map = true;
		mesh->material->set_tex_diffuse(parms.diff_path);
		// Rebuild Material
		if (parms.use_diff) mesh->material->build_mesh_shader();
	}

	// ======== Spec Map
	ImGui::Separator();
	ImGui::Text("Specular Map");
	if (ImGui::Checkbox("Use Spec", &parms.use_spec)) {
		// Update Material
		mesh->material->build_mesh_shader();
	}
	// Spec Map Thumbnail
	ImVec4 spec_border = parms.use_spec ? ImVec4(0.f, 1.f, 0.f, .5f) : ImVec4(1.f, 1.f, 1.f, .5f);
	ImTextureID s_tex = parms.use_spec ? parms.thumb_spec->get_imgui_texture() : 0;
	ImGui::Image(s_tex, ImVec2(64, 64), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), spec_border);
	// Spec Map File
	ImGui::InputText("1", parms.spec_buf, 512);
	ImGui::SameLine();
	if (ImGui::Button("Load Spec Map")) 
	{
		parms.spec_path = parms.spec_buf;
		// Update Thumb
		if (parms.thumb_spec) delete parms.thumb_spec;
		parms.thumb_spec = new Fyredon::TextureImage(parms.spec_buf, "thumb_spec", 2, true); parms.thumb_spec->load();
		// Update Material
		mesh->material->parms.use_spec_map = true;
		mesh->material->set_tex_specular(parms.spec_path);
		// Rebuild Material
		if (parms.use_spec) mesh->material->build_mesh_shader();
	}
	// ======== Bump Map
	ImGui::Separator();
	ImGui::Text("Bump Map");
	if (ImGui::Checkbox("Use Bump", &parms.use_bump)) {
		// Update Material
		mesh->material->build_mesh_shader();
	}
	// Bump Map Thumbnail
	ImVec4 bump_border = parms.use_bump ? ImVec4(0.f, 1.f, 0.f, .5f) : ImVec4(1.f, 1.f, 1.f, .5f);
	ImTextureID b_tex = parms.use_bump ? parms.thumb_bump->get_imgui_texture() : 0;
	ImGui::Image(b_tex, ImVec2(64, 64), ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f), ImVec4(1.0f, 1.0f, 1.0f, 1.0f), bump_border);
	// Bump Map File
	ImGui::InputText("2", parms.bump_buf, 512);
	ImGui::SameLine();
	if (ImGui::Button("Load Bump Map"))
	{
		parms.bump_path = parms.bump_buf;
		// Update Thumb
		if (parms.thumb_bump) delete parms.thumb_bump;
		parms.thumb_bump = new Fyredon::TextureImage(parms.bump_buf, "thumb_bump", 2, true); parms.thumb_bump->load();
		// Update Material
		mesh->material->parms.use_bump_map = true;
		mesh->material->set_tex_bump(parms.bump_path);
		// Rebuild Material
		if (parms.use_bump) mesh->material->build_mesh_shader();
	}
	// ================ Material Parms ================
	// Info : No need to rebuild material shaders for these parms (uniforms, not branches).
	// ======== Ambient Term
	ImGui::Separator();
	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(200, 200, 200, 255));
	ImGui::Text("Ambient");
	ImGui::PopStyleColor();
	if (ImGui::SliderFloat3("Ambient_Col", col_amb, 0.f, 1.f)) {
		mesh->material->parms.colour_ambient = glm::vec3(col_amb[0], col_amb[1], col_amb[2]);
	}
	ImGui::SliderFloat("Ambient_Strength", &mesh->material->parms.coeff_ambient, 0.f, 10.f);
	// ======== Diffuse Term
	ImGui::Separator();
	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(200, 200, 200, 255));
	ImGui::Text("Diffuse");
	ImGui::PopStyleColor();
	if (ImGui::SliderFloat3("Diffuse_Col", col_dif, 0.f, 1.f)) {
		mesh->material->parms.colour_diffuse = glm::vec3(col_dif[0], col_dif[1], col_dif[2]);
	}
	if (ImGui::Checkbox("Use Vert Colour", &mesh->material->parms.use_vert_col)) {
		mesh->material->build_mesh_shader();
	}
	ImGui::SliderFloat("Diffuse_Strength", &mesh->material->parms.coeff_diffuse, 0.f, 25.f);
	// ======== Specular Term
	ImGui::Separator();
	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(200, 200, 200, 255));
	ImGui::Text("Specular");
	ImGui::PopStyleColor();
	if (ImGui::SliderFloat3("Specular_Col", col_spc, 0.f, 1.f)) {
		mesh->material->parms.colour_specular = glm::vec3(col_spc[0], col_spc[1], col_spc[2]);
	}
	ImGui::SliderFloat("Specular_Stength", &mesh->material->parms.coeff_specular, 0.f, 25.f);
	ImGui::SliderFloat("Specular_Exp",     &mesh->material->parms.exp_gloss,     0.f, 25.f);
	// ======== Bump Term
	ImGui::SliderFloat("Bump_Strength", &mesh->material->parms.coeff_bump, 0.f, 1.f);

	// ======== Shader Code
	if (ImGui::Button("Show Shader Code"))
	{
		parms.show_shader = !parms.show_shader; 
	}
	// If Show Shader Code 
	ImGui::Separator();
	if (parms.show_shader)
	{
		ImGui::Begin("Uber Shader Code - BlinnPhongLambertian");

		ImGui::Dummy(ImVec2(0.0f, 10.0f));
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(250, 200, 150, 255));
		ImGui::PopStyleColor();
		ImGui::Text(mesh->shader.frag_shader_code.c_str());

		ImGui::End();
	}

	// End Mesh Component GUI
	ImGui::Separator();
	ImGui::Separator();
}

// Info : Update Camera State per tick (No GUI Tick here, this is done via Component::tick_gui() in editortick)
void mesh_component::tick()
{
	//
}