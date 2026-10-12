#include "Fyredon/Other/pch.h"
#include "Fyredon/ECS/mesh_component.h"
#include "Fyredon/ECS/fbx_entity.h"
#include "Fyredon/ECS/navigation_component.h"
#include "Fyredon/ECS/transform_component.h"
#include <filesystem>

mesh_component::mesh_component(Fyredon::Mesh* mesh)
{
	this->componentName = "Mesh";
	this->mesh = mesh;
	this->mesh_render = new Fyredon::Mesh(*mesh);
	Fyredon::Renderer::get_renderer().submit_mesh(this->mesh_render);
	this->memmory_size = sizeof(mesh_component);
    if(auto* obj=dynamic_cast<Fyredon::Mesh_OBJ*>(mesh))
        strncpy_s(modelPath,sizeof(modelPath),obj->filePath().c_str(),_TRUNCATE);
    else if(auto* fbx=dynamic_cast<fbx_mesh*>(mesh))
        strncpy_s(modelPath,sizeof(modelPath),fbx->filePath().c_str(),_TRUNCATE);
}

// Remove mesh from render_list if component deleted
mesh_component::mesh_component::~mesh_component()
{
	//auto& ren = Fyredon::Renderer::get_renderer();
	//ren.delete_mesh ...;
}

void mesh_component::GUI_struct()
{
	    if(ImGui::InputText("Model path",modelPath,sizeof(modelPath),ImGuiInputTextFlags_EnterReturnsTrue))
        pendingReload=true;
    if(ImGui::Button("Load model"))pendingReload=true;
    ImGui::TextUnformatted("OBJ / FBX | relative to project or absolute path");
    if(!modelError.empty())ImGui::TextWrapped("%s",modelError.c_str());
    std::string vertex = "Vertex: " + std::to_string(this->mesh->vert_count);
	std::string face = "Face: " + std::to_string(this->mesh->index_count / 3);
	ImGui::Text(vertex.data());
	ImGui::Text(face.data());
}

void mesh_component::tick()
{
	if(pendingReload){pendingReload=false;loadModel(modelPath);}
	
}

void mesh_component::render()
{
	this->renderGUI();
	this->mesh_render->render();
}

void mesh_component::changeMesh(Fyredon::Mesh* mesh, bool IsReplace = false)
{
	if (IsReplace)
		free(this->mesh);
	this->mesh = mesh;
	if (this->mesh_render)free(this->mesh_render);
	this->mesh_render = new Fyredon::Mesh(*mesh);
}

bool mesh_component::loadModel(const std::string& file)
{
    try {
        namespace fs=std::filesystem;
        auto path=fs::u8path(file);
        if(path.empty())throw std::runtime_error("Enter a model path.");
        if(path.is_relative()){
            const auto rooted=fs::current_path().parent_path()/path;
            path=fs::is_regular_file(rooted)?rooted:fs::absolute(path);
        }
        if(!fs::is_regular_file(path))throw std::runtime_error("Model file not found.");
        auto extension=path.extension().u8string();
        std::transform(extension.begin(),extension.end(),extension.begin(),[](unsigned char c){return static_cast<char>(std::tolower(c));});
        const auto utf8=path.u8string();
        std::unique_ptr<Fyredon::Mesh> candidate;
        if(extension==".obj"){
            auto obj=std::make_unique<Fyredon::Mesh_OBJ>(mesh->name.c_str(),utf8.c_str());
            obj->load_obj();candidate=std::move(obj);
        }else if(extension==".fbx"){
            auto fbx=std::make_unique<fbx_mesh>(mesh->name.c_str(),utf8.c_str());
            fbx->load_obj();candidate=std::move(fbx);
        }else throw std::runtime_error("Supported model formats: .obj and .fbx.");
        if(!candidate->vert_count||!candidate->index_count)
            throw std::runtime_error("Model contains no renderable triangles.");
        candidate->shader=mesh->shader;candidate->flags.shader_set=mesh->flags.shader_set;
        candidate->model=mesh->model;
        candidate->uv_offset=mesh->uv_offset;candidate->uv_scale=mesh->uv_scale;
        candidate->use_tex=mesh->use_tex;candidate->flags.texture_set=mesh->flags.texture_set;
        // Texture component keeps its existing resource; avoid ownership duplication.
        auto draw=std::make_unique<Fyredon::Mesh>(*candidate);
        candidate->tex=mesh->tex;draw->tex=mesh->tex;
        draw->model=mesh_render->model;
        auto* oldMesh=mesh;auto* oldDraw=mesh_render;
        auto& renderer=Fyredon::Renderer::get_renderer();
        renderer.remove_mesh(oldDraw);
        mesh=candidate.release();mesh_render=draw.release();renderer.submit_mesh(mesh_render);
        if(auto* fbxEntity=dynamic_cast<fbx_entity*>(parent))fbxEntity->FBXmesh=dynamic_cast<fbx_mesh*>(mesh);
        if(auto* animation=dynamic_cast<fbx_property_component*>(parent->getComponentByName("Animation")))
            animation->meshChanged();
        else if(dynamic_cast<fbx_mesh*>(mesh)){
            auto* animation=new fbx_property_component();
            parent->addComponent(animation);
            if(parent->getComponentByName("Move test"))animation->setNavigationControlled(true);
        }
        if(auto* navigation=dynamic_cast<navigation_component*>(parent->getComponentByName("Navigation"))){
            navigation->mesh=mesh;navigation->getMeshDetail();navigation->refreshDetail();navigation->reset();
        }
        // Draw copy shares old GPU buffers; only the source mesh releases them.
        oldDraw->VAO=oldDraw->VBO=oldDraw->EBO=0;oldDraw->tex=nullptr;
        oldMesh->tex=nullptr;
        delete oldDraw;delete oldMesh;
        strncpy_s(modelPath,sizeof(modelPath),file.c_str(),_TRUNCATE);
        modelError.clear();return true;
    }catch(const std::exception& error){modelError=error.what();return false;}
}