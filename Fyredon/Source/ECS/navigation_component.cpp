#include "Fyredon/Other/pch.h"
#include "Fyredon/ECS/navigation_component.h"
#include "Fyredon/Navigation/NavigationMapFile.h"
#include <filesystem>
#include <random>
// the parent of this component must contain mesh_component
navigation_component::navigation_component(Entity* parent)
{
	this->componentName = "Navigation";
	this->parent = parent;
	if (this->parent->getComponentByName("Mesh") != nullptr) {
		this->mesh = ((mesh_component*)this->parent->getComponentByName("Mesh"))->mesh;
		getMeshDetail();
	}
	else {
		std::cout << "Error: parent do not contain entity [Mesh]" << std::endl;
	}
	this->memmory_size = sizeof(navigation_component);

	////////////wall test///////////////
	wall = new Fyredon::Mesh_OBJ("wall", "..//Assets//Mesh//wall.obj");
	wall->load_obj();
	wall->set_shader("..//Shaders//mesh.vert", "..//Shaders//mesh.frag");
	wall->set_colour(glm::vec3(0.0f, 0.0f, 1.0f));
	wall->mode = Fyredon::Render_Mode::RENDER_MESH;
	wall->scale(glm::vec3(1.f, 2.f, 1.f));

}

void navigation_component::GUI_struct()
{
    ImGui::TextUnformatted("Map: Assets/script/map.txt");
    ImGui::InputFloat("Cell size", &grid_width, 0.1f);
    ImGui::Text("Grid: %d x %d | Loaded cell size: %.2f", widthCount, lengthCount, loadedGridWidth);
    ImGui::TextUnformatted("0 Ground | 1 Wall | 2 Start | 3 Goal");
        ImGui::Separator();
    ImGui::InputText("Wall OBJ directory", wallDirectory, sizeof(wallDirectory));
    ImGui::InputText("Model ID", wallModelId, sizeof(wallModelId));
    ImGui::TextUnformatted("ID >= 0: modulo model count | ID < 0: random");
    ImGui::Checkbox("Random 90-degree rotation", &randomWallRotation);
    ImGui::InputFloat3("Wall offset", &wallOffset.x);
    ImGui::InputFloat3("Wall scale", &wallScale.x);
    ImGui::TextWrapped("Models fit the cell footprint and rest on the ground. Offset is in world units; scale is a multiplier. Apply with Reload map.");
    if(ImGui::TreeNode("Loaded model IDs")) {
        for(size_t i=0;i<wallModelNames.size();++i)ImGui::Text("%zu: %s",i,wallModelNames[i].c_str());
        ImGui::TreePop();
    }
    if (ImGui::Button("Reload map")) loadMap();
    if (!mapError.empty()) ImGui::TextWrapped("%s", mapError.c_str());
}

bool navigation_component::loadMap(const std::string& file)
{
    try {
        if (!std::isfinite(grid_width) || grid_width < 0.1f || grid_width > 100.f)
            throw std::runtime_error("Cell size must be between 0.1 and 100.");
        std::ifstream input(std::filesystem::u8path(file), std::ios::binary);
        if (!input) throw std::runtime_error("Cannot open Assets/script/map.txt.");
        const auto data = parseNavigationMap(std::string(std::istreambuf_iterator<char>(input), {}));
        const int size = static_cast<int>(data.cells.size());
        auto nextMap = std::make_unique<AStarMap>(size, size);
        std::vector<Pos> nextWalls;
        for (int z=0; z<size; ++z) for (int x=0; x<size; ++x) {
            if (data.cells[z][x]==1) { nextMap->addWall(Pos(x,z)); nextWalls.emplace_back(x,z); }
        }
        prepareWalls(nextWalls, size);
        // Invalid files never interrupt the existing scene. A valid reload starts a new session.
        if (beforeMapReload) beforeMapReload();
        reset();
        delete map; map=nextMap.release(); walls=std::move(nextWalls);
        widthCount=lengthCount=size;
        loadedGridWidth=grid_width;
        width_grid=length_grid=size*loadedGridWidth;
        min_X_grid=min_Z_grid=-width_grid/2.f;
        max_X_grid=max_Z_grid=width_grid/2.f;
        auto* floorTransform=static_cast<transform_component*>(parent->getComponentByName("Transform"));
        floorTransform->reset();
        for(int i=0;i<3;++i){floorTransform->matrixTranslation[i]=0;floorTransform->matrixRotation[i]=0;}
        floorTransform->matrixScale[0]=width_grid/width;
        floorTransform->matrixScale[1]=1;
        floorTransform->matrixScale[2]=length_grid/length;
        floorTransform->isChanged=true;
        auto place=[this](Entity* entity, int x,int z) {
            if(!entity)return;
            entity->reset();
            auto* tr=static_cast<transform_component*>(entity->getComponentByName("Transform"));
            const auto p=Pos2Position(Pos(x,z));
            tr->matrixTranslation[0]=p.x;tr->matrixTranslation[2]=p.y; // Keep the object's vertical offset.
            tr->isChanged=true;
        };
        place(character,data.startX,data.startZ);
        place(destination,data.endX,data.endZ);
        walls2renderer();
        mapError.clear();
        std::cout<<"Loaded navigation map: "<<size<<" x "<<size<<", walls="<<walls.size()<<std::endl;
        return true;
    } catch(const std::exception& error) {
        mapError=error.what();
        std::cerr<<"Navigation map: "<<mapError<<std::endl;
        return false;
    }
}
void navigation_component::createGrid()
{
	refreshDetail();
    // set grid width(larger than 0 and smaller than width or length)
	if (this->grid_width < this->min_width) {
		this->grid_width = this->min_width;
	}
	
	float maxWidth = (this->width_tran < this->length_tran) ? this->width_tran : this->length_tran;
	if (this->grid_width > maxWidth) {
		this->grid_width = maxWidth;
	}
	// get middle point
	this->widthCount = 2 * (int)((this->width_tran - this->grid_width) / (2.0f * this->grid_width)) + 3;
	this->lengthCount = 2 * (int)((this->length_tran - this->grid_width) / (2.0f * this->grid_width)) + 3;
	this->min_X_grid = this->mid_X_tran - (this->widthCount - 1) * this->grid_width / 2 - this->grid_width / 2;
	this->max_X_grid = this->mid_X_tran + (this->widthCount - 1) * this->grid_width / 2 + this->grid_width / 2;
	this->min_Z_grid = this->mid_Z_tran - (this->lengthCount - 1) * this->grid_width / 2 - this->grid_width / 2;
	this->max_Z_grid = this->mid_Z_tran + (this->lengthCount - 1) * this->grid_width / 2 + this->grid_width / 2;
	this->width_grid = this->max_X_grid - this->min_X_grid;
	this->length_grid = this->max_Z_grid - this->min_Z_grid;
	delete this->map;
    this->loadedGridWidth = grid_width;
    this->map = new AStarMap(this->widthCount, this->lengthCount);
}

void navigation_component::getMeshDetail() 
{
	if (this->mesh->vert_data.size() > 0) {
		this->min_X = this->mesh->vert_data[0].pos[0];
		this->max_X = this->mesh->vert_data[0].pos[0];
		this->min_Z = this->mesh->vert_data[0].pos[2];
		this->max_Z = this->mesh->vert_data[0].pos[2];
	}
	for (int i = 0; i < this->mesh->vert_data.size(); i++) {
		if (this->mesh->vert_data[i].pos[0] < this->min_X) {
			this->min_X = this->mesh->vert_data[i].pos[0];
		}
		if (this->mesh->vert_data[i].pos[0] > this->max_X) {
			this->max_X = this->mesh->vert_data[i].pos[0];
		}
		if (this->mesh->vert_data[i].pos[2] < this->min_Z) {
			this->min_Z = this->mesh->vert_data[i].pos[2];
		}
		if (this->mesh->vert_data[i].pos[2] > this->max_Z) {
			this->max_Z = this->mesh->vert_data[i].pos[2];
		}
	}
	this->width = this->max_X - this->min_X;
	this->length = this->max_Z - this->min_Z;
	this->mid_X = this->min_X + this->width / 2;
	this->mid_Z = this->min_Z + this->length / 2;
}

void navigation_component::refreshDetail()
{
	float translateX = ((transform_component*)this->parent->getComponentByName("Transform"))->matrixTranslation[0];
	float translateZ = ((transform_component*)this->parent->getComponentByName("Transform"))->matrixTranslation[2];
	float scaleX = ((transform_component*)this->parent->getComponentByName("Transform"))->matrixScale[0];
	float scaleZ = ((transform_component*)this->parent->getComponentByName("Transform"))->matrixScale[2];
	this->min_X_tran = scaleX * this->min_X;
	this->min_Z_tran = scaleZ * this->min_Z;
	this->max_X_tran = scaleX * this->max_X;
	this->max_Z_tran = scaleZ * this->max_Z;
	this->width_tran = this->max_X_tran - this->min_X_tran;
	this->length_tran = this->max_Z_tran - this->min_Z_tran;
	this->min_X_tran = translateX + this->min_X_tran;
	this->max_X_tran = translateX + this->max_X_tran;
	this->min_Z_tran = translateZ + this->min_Z_tran;
	this->max_Z_tran = translateZ + this->max_Z_tran;
	this->mid_X_tran = this->min_X_tran + this->width_tran / 2;
	this->mid_Z_tran = this->min_Z_tran + this->length_tran / 2;
}

list<glm::vec2> navigation_component::findPath(glm::vec2 src, glm::vec2 dest)
{
	debugPath.clear(); ++pathRevision;
    glm::vec2 srcIndex = pos2index(src[0], src[1]);
	glm::vec2 destIndex = pos2index(dest[0], dest[1]);
    if (!map || srcIndex.x<0 || srcIndex.y<0 || destIndex.x<0 || destIndex.y<0) {
        delete posList; posList=nullptr;
        return {};
    }
	list<Pos>* poslist = this->map->find(Pos(srcIndex[0], srcIndex[1]), Pos(destIndex[0], destIndex[1]));
	delete this->posList;
	this->posList = poslist;
	list<glm::vec2> position = poslist ? Pos2Position(poslist) : list<glm::vec2>{};
    if (!position.empty()) {
        debugPath.push_back(src);
        debugPath.insert(debugPath.end(), position.begin(), position.end());
        debugPath.push_back(dest);
    }
	return position;
}

glm::vec2 navigation_component::pos2index(float x, float y)
{
	int xIndex, yIndex;
	float width = this->width_grid;
	int xCount = this->widthCount;
	float length = this->length_grid;
	int yCount = this->lengthCount;
	if (x < this->min_X_grid || x >= this->max_X_grid) {
		xIndex = -1;
	}
	else {
		xIndex = (x - this->min_X_grid) / (width / xCount);
	}
	if (y < this->min_Z_grid || y >= this->max_Z_grid) {
		yIndex = -1;
	}
	else {
		yIndex = (y - this->min_Z_grid) / (length / yCount);
	}
	glm::vec2 index(xIndex, yIndex);
	return index;
}

list<glm::vec2> navigation_component::Pos2Position(list<Pos>* pos)
{
	list<glm::vec2> position;
	for (Pos p : *pos) {
		position.push_back(Pos2Position(p));
	}
	return position;
}

glm::vec2 navigation_component::Pos2Position(Pos pos)
{
	glm::vec2 pt(
		pos.x * this->loadedGridWidth + this->loadedGridWidth / 2 + this->min_X_grid,
		pos.y * this->loadedGridWidth + this->loadedGridWidth / 2 + this->min_Z_grid);
	return pt;
}

void navigation_component::addWall(int x, int y)
{
	this->map->addWall(Pos(x, y));
}

void navigation_component::walls2renderer()
{
    auto& renderer=Fyredon::Renderer::get_renderer();
    if(wallsSubmitted){renderer.remove_mesh(wallDisplay);wallsSubmitted=false;}
    if(walls.empty()) return;
    if(!wallDisplay) {
        wallDisplay=new Fyredon::Mesh("Navigation walls");
        wallDisplay->set_shader("../Shaders/mesh.vert","../Shaders/mesh.frag");
    }
    wallDisplay->set_data_mesh(preparedWallVertices);
    renderer.submit_mesh(wallDisplay);wallsSubmitted=true;
}
void navigation_component::reset()
{
    Component::reset();
    path.clear();
    debugPath.clear(); ++pathRevision;
    delete posList;
    posList = nullptr;
}
void navigation_component::prepareWalls(const std::vector<Pos>& cells, int size)
{
    namespace fs=std::filesystem;
    for(int i=0;i<3;++i) {
        if(!std::isfinite(wallOffset[i])||!std::isfinite(wallScale[i])||wallScale[i]<=0)
            throw std::runtime_error("Wall offset must be finite; scale must be finite and positive.");
    }
    auto directory=fs::u8path(wallDirectory);
    if(directory.is_relative())directory=fs::current_path().parent_path()/directory;
    if(!fs::is_directory(directory))throw std::runtime_error("Wall OBJ directory does not exist.");
    std::vector<fs::path> files;
    for(const auto& entry:fs::directory_iterator(directory)) {
        auto extension=entry.path().extension().u8string();
        std::transform(extension.begin(),extension.end(),extension.begin(),[](unsigned char c){return static_cast<char>(std::tolower(c));});
        if(entry.is_regular_file()&&extension==".obj")files.push_back(entry.path());
    }
    std::sort(files.begin(),files.end(),[](const fs::path& a,const fs::path& b){return a.filename().u8string()<b.filename().u8string();});
    if(files.empty())throw std::runtime_error("Wall directory contains no OBJ files.");
    // Parse decimal digits modulo count without overflowing, even for very large IDs.
    std::string id=wallModelId;
    size_t begin=id.find_first_not_of(" \t\r\n"),end=id.find_last_not_of(" \t\r\n");
    if(begin==std::string::npos)throw std::runtime_error("Model ID must be an integer.");
    id=id.substr(begin,end-begin+1);
    bool negative=id[0]=='-',nonzero=false;
    size_t digit=(id[0]=='-'||id[0]=='+')?1:0,selected=0;
    if(digit==id.size())throw std::runtime_error("Model ID must be an integer.");
    for(;digit<id.size();++digit){
        if(id[digit]<'0'||id[digit]>'9')throw std::runtime_error("Model ID must be an integer.");
        nonzero=nonzero||id[digit]!='0';
        selected=(selected*10+(id[digit]-'0'))%files.size();
    }
    std::vector<std::unique_ptr<Fyredon::Mesh_OBJ>> models;
    std::vector<glm::vec3> origins;
    std::vector<float> fits;
    std::vector<std::string> names;
    for(const auto& file:files) {
        const auto path=file.u8string(),name=file.filename().u8string();
        auto model=std::make_unique<Fyredon::Mesh_OBJ>(name.c_str(),path.c_str());
        model->load_obj();
        if(model->vert_data.empty()||model->inds_data.empty())throw std::runtime_error("Empty or invalid wall OBJ: "+name);
        auto lo=model->vert_data[0].pos,hi=lo;
        for(const auto& v:model->vert_data){
            for(int i=0;i<3;++i)if(!std::isfinite(v.pos[i]))throw std::runtime_error("Invalid OBJ coordinates.");
            lo=glm::min(lo,v.pos);hi=glm::max(hi,v.pos);
        }
        const float span=std::max(hi.x-lo.x,hi.z-lo.z);
        if(span<1e-6f)throw std::runtime_error("Wall OBJ has zero footprint: "+name);
        origins.emplace_back((lo.x+hi.x)*0.5f,lo.y,(lo.z+hi.z)*0.5f);
        fits.push_back(grid_width/span);names.push_back(name);models.push_back(std::move(model));
    }
    std::mt19937 random(std::random_device{}());
    std::uniform_int_distribution<size_t> choose(0,files.size()-1);
    std::uniform_int_distribution<int> turn(0,3);
    std::vector<Fyredon::Vert> vertices;
    std::vector<size_t> ids;
    std::vector<int> turns;
    for(auto cell:cells) {
        const auto idIndex=negative&&nonzero?choose(random):selected;
        const int rotation=randomWallRotation?turn(random):0;
        ids.push_back(idIndex);turns.push_back(rotation);
        auto rotate=[rotation](glm::vec3 v){
            for(int i=0;i<rotation;++i)v=glm::vec3(v.z,v.y,-v.x);
            return v;
        };
        const glm::vec3 center((cell.x+0.5f-size*0.5f)*grid_width,0,(cell.y+0.5f-size*0.5f)*grid_width);
        for(auto index:models[idIndex]->inds_data){
            auto v=models[idIndex]->vert_data[index];
            v.pos=rotate((v.pos-origins[idIndex])*fits[idIndex]*wallScale)+center+wallOffset;
            auto normal=rotate(v.normal/wallScale);
            v.normal=glm::length(normal)>1e-6f?glm::normalize(normal):glm::vec3(0,1,0);
            v.col=glm::vec3(0.55f,0.58f,0.6f);
            vertices.push_back(v);
        }
    }
    preparedWallVertices=std::move(vertices);
    wallModelNames=std::move(names);
    selectedWallModels=std::move(ids);selectedWallTurns=std::move(turns);
}