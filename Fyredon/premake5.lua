project "Fyredon"
	kind "SharedLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off" -- multithreaded DLL (MD) in VS


	targetdir("%{wks.location}/bin/" .. outdir .. "/%{prj.name}")
	objdir("%{wks.location}/bin-obj/" .. outdir .. "/%{prj.name}")
	
	files{
		"Source/**/**.cpp",    -- Source/<dir>/*.cpp
		"Source/**/**/**.cpp", -- Source/extern/dearimgui/*.cpp
		"Source/**/**/**.c",   -- extern/lua-5.4.4/
		"Include/**/**.h"
	}

	includedirs{
		"%{wks.location}/Fyredon/Include/",
		"%{wks.location}/Extern/Include/",
		"%{wks.location}/Extern/Include/dearimgui/", -- need abs dearimgui dir, for cases of : #include "imgui.h" etc.
		"%{wks.location}/Extern/Include/FBX/",        -- need abs fbx dir, for cases of : #include "fbxsdk/fbxsdk/fbxsdk_version.h"
		"%{wks.location}/Extern/Include/lua-5.4.4",
		"%{wks.location}/Extern/Include/LuaBridge"
	}

	-- precompiled headers
	pchheader "Fyredon/Other/pch.h"
	pchsource "Source/Other/pch.cpp"
	
	-- Linking to static libaries of GLEW and GLFW for now. Sharjeel, you can look into submodules or something better :) 
	libdirs { 
		"%{wks.location}/Extern/Binary/GLEW/",
		"%{wks.location}/Extern/Binary/GLFW/",
		"%{wks.location}/Extern/Binary/FBX/"
	}
	
	links{
		"glew32s.lib",
		"glfw3.lib",
		"opengl32.lib",
		"libfbxsdk.lib"
		}

	-- Windows system settings
	filter "system:windows"
		systemversion "latest"

		defines{
			"FD_BUILD_DLL", -- to define the dll import/export api
			"FD_ROOT"
		}

		-- Post build step: copy DLLs generated into other project binaries
		postbuildcommands{
			("{COPY} ../bin/" .. outdir .. "/%{prj.name}/*.dll ../bin/" .. outdir .. "/Client"),
		}

	-- Debug configuration
	filter "configurations:Debug"
		defines{
			"FD_DEBUG",
			"GLEW_STATIC",
			"FBXSDK_SHARED"
		}
		runtime "Debug"
		symbols "on"


	-- Release configuration
	filter "configurations:Release"
		defines{
			"FD_RELEASE",
			"GLEW_STATIC",
			"FBXSDK_SHARED"
		}

		runtime "Release"
		optimize "on"

	filter "files:Source/Extern/lua-5.4.4/**.c"
        flags { "NoPCH" }