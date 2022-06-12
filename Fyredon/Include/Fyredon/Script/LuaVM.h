#pragma once
#include "Fyredon/Other/pch.h"
extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
#include "LuaBridge/LuaBridge.h"


#include <string>

static int ApluseB(int a, int b) {
	return a + b;
}

static int printInt(lua_State* L) {
	int input1 = lua_tointeger(L, 1);
	int input2 = lua_tointeger(L, 2);
	int output = ApluseB(input1, input2);
	lua_pushstring(L, std::string("output").data());
	lua_pushinteger(L, output);
	return 2;
}

namespace Fyredon
{
	class LuaVM
	{
	public:
		LuaVM();
		~LuaVM();
		inline auto GetState() { return L; }

		// Load Script
		bool LoadScript(const::std::string& v);
		bool LoadScript();
		bool SetScriptFile(const::std::string& v);

		// Call Lua Func
		template<typename R>
		R Call(const char* name)
		{
			lua_getglobal(L, name);
			if (lua_isfunction(L, -1))
			{
				//call func£¨parameters are lua, param num£¬return_val num£¬err return£©
				lua_pcall(L, 0, 1, 0);
			}
			return luabridge::Stack<R>::get(L, -1);
		}

		template<typename R, typename T1>
		R Call(const char* name, T1 arg1)
		{
			lua_getglobal(L, name);
			if (lua_isfunction(L, -1))
			{
				//push parameters
				luabridge::Stack<T1>::push(L, arg1);
				//call func£¨parameters are lua, param num£¬return_val num£¬err return£©
				lua_pcall(L, 1, 1, 0);
			}
			return luabridge::Stack<R>::get(L, -1);
		}

		template<typename R, typename T1, typename T2>
		R Call(const char* name, T1 arg1, T2 arg2)
		{
			//load func name
			lua_getglobal(L, name);
			if (lua_isfunction(L, -1))
			{
				//push parameters
				luabridge::Stack<T1>::push(L, arg1);
				luabridge::Stack<T2>::push(L, arg2);
				//call func£¨parameters are lua, param num£¬return_val num£¬err return£©
				lua_pcall(L, 2, 1, 0);
			}
			//get return_val
			return luabridge::Stack<R>::get(L, -1);
		}

		template<typename R, typename T1, typename T2, typename T3>
		R Call(const char* name, T1 arg1, T2 arg2, T3 arg3)
		{
			lua_getglobal(L, name);
			if (lua_isfunction(L, -1))
			{
				luabridge::Stack<T1>::push(L, arg1);
				luabridge::Stack<T2>::push(L, arg2);
				luabridge::Stack<T3>::push(L, arg3);
				lua_pcall(L, 3, 1, 0);
			}
			return luabridge::Stack<R>::get(L, -1);
		}
	private:
		void Initialize();
		void Destroy();
		//void AddSystemPath(const std::string& path);
		//void AddPath(const std::string& path);
		lua_State* L = nullptr;
		std::string _file;
	};
};


//int demo_lua() {
//	Fyredon::LuaVM luaVM;
//
//	luaVM.LoadScript("test.lua");
//	//luaVM.Call();
//	std::cout << "omgomg" << std::endl;
//	return 0;
//}