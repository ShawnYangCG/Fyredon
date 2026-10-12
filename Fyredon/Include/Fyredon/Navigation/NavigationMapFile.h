#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <cctype>
struct NavigationMapData {
    std::vector<std::vector<int>> cells;
    int startX=0,startZ=0,endX=0,endZ=0;
};
inline NavigationMapData parseNavigationMap(std::string text) {
    if(text.compare(0,3,"\xEF\xBB\xBF")==0) text.erase(0,3);
    auto trim=[](std::string s) {
        const auto a=s.find_first_not_of(" \t\r\n");
        return a==std::string::npos?std::string():s.substr(a,s.find_last_not_of(" \t\r\n")-a+1);
    };
    text=trim(text);
    if(!text.empty()&&text.back()==';')text=trim(text.substr(0,text.size()-1));
    if(text.empty())throw std::runtime_error("Map is empty.");
    if(text.back()==';')throw std::runtime_error("Empty row.");
    NavigationMapData data;int starts=0,ends=0;
    std::istringstream rows(text);std::string row;
    while(std::getline(rows,row,';')){
        row=trim(row);
        if(row.empty()||row.back()==',')throw std::runtime_error("Empty row or cell.");
        std::istringstream columns(row);std::string value;std::vector<int> cells;
        while(std::getline(columns,value,',')){
            value=trim(value);
            if(value.size()!=1||value[0]<'0'||value[0]>'3')
                throw std::runtime_error("Each cell must be 0, 1, 2 or 3; use commas and semicolons.");
            int v=value[0]-'0',x=static_cast<int>(cells.size()),z=static_cast<int>(data.cells.size());
            if(v==2){++starts;data.startX=x;data.startZ=z;}
            if(v==3){++ends;data.endX=x;data.endZ=z;}
            cells.push_back(v);
            if(cells.size()>256)throw std::runtime_error("Map exceeds 256 x 256.");
        }
        data.cells.push_back(cells);
        if(data.cells.size()>256)throw std::runtime_error("Map exceeds 256 x 256.");
    }
    for(const auto& r:data.cells)if(r.size()!=data.cells.size())throw std::runtime_error("Map must be a square with equal row lengths.");
    if(starts!=1||ends!=1)throw std::runtime_error("Map requires exactly one start (2) and one goal (3).");
    return data;
}
