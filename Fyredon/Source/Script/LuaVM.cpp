
#include "Fyredon/Other/pch.h"
#include <Fyredon/Script/LuaVM.h>
#include "LuaBridge/LuaBridge.h"
#include <functional>
#include <filesystem>


// Include export head file
#include <Fyredon/Script/LoggerExport.h>
#include <Fyredon/Script/MathExport.h>

namespace Fyredon
{

	LuaVM::LuaVM()
	{
		Initialize();
	}

	LuaVM::~LuaVM()
	{
		Destroy();
	}

	void LuaVM::Initialize()
	{
		L = luaL_newstate();
		luaL_openlibs(L);
		//AddPath(".");
		
		// Add export to lua c code here
		LoggerExport::Export(L);
		MathExport::Export(L);

		lua_pushcfunction(L, printInt);
		lua_setglobal(L, "myPrint");
	
	}

	void LuaVM::Destroy()
	{
		lua_close(L);
	}


	bool LuaVM::SetScriptFile(const std::string& v)
	{
		_file = v;
		return true;
	}

	bool LuaVM::LoadScript(const::std::string& v)
	{
		_file = v;
		return LoadScript();
	}

	bool LuaVM::LoadScript()
	{
		bool result = luaL_dofile(GetState(), _file.c_str());
		return result;
	}

	//void LuaVM::AddSystemPath(const std::string& path)
	//{
	//	std::string v;
	//	lua_getglobal(L, "package");
	//	lua_getfield(L, -1, "path");
	//	v.append(lua_tostring(L, -1));
	//	v.append(";");
	//	v.append(path.c_str());
	//	lua_pushstring(L, v.c_str());
	//	lua_setfield(L, -3, "path");
	//	lua_pop(L, 2);
	//}

	//void LuaVM::AddPath(const std::string& path)
	//{
	//	AddSystemPath(path + "/?.lua");
	//
	//	for (const auto& entry : std::filesystem::directory_iterator(path))
	//	{
	//		bool isDir = std::filesystem::is_directory(entry);
	//		if (isDir) {
	//			AddPath(entry.path().string());
	//		}
	//	}
	//}

};
