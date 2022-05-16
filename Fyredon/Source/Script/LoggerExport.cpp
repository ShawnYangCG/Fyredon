#include "Fyredon/Other/pch.h"
#include <Fyredon/Script/LuaVM.h>
#include <Fyredon/Script/LoggerExport.h>
#include <LuaBridge/LuaBridge.h>
#include <Fyredon/Debug/Logger.h>


namespace Fyredon
{
	namespace LoggerExport
	{

		void Export(lua_State* L)
		{
			luabridge::getGlobalNamespace(L)
				.addFunction("FD_DEBUG_INFO", std::function <void(const char*, const char*)>([](const char* fmt, const char* str) {
				        FD_DEBUG_INFO(fmt, str); 
					}))
				.addFunction("FD_DEBUG_WARN", std::function <void(const char*, const char*)>([](const char* fmt, const char* str) {
						FD_DEBUG_WARN(fmt, str);
					}))
				.addFunction("FD_DEBUG_ERROR", std::function <void(const char*, const char*)>([](const char* fmt, const char* str) {
						FD_DEBUG_ERROR(fmt, str);
				}));

		}
	};
};