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
		"Include/Fyredon/**/**.h"
	}

	includedirs{
		"%{wks.location}/Fyredon/Include/",
		"%{wks.location}/Extern/Include/",
		"%{wks.location}/Extern/Include/dearimgui/" -- need dearimgui dir, for its own includes #include "imgui.h" etc.
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

	-- Debug configuration
	filter "configurations:Debug"
		defines{
			"FD_DEBUG"
		}
		runtime "Debug"
		symbols "on"


	-- Release configuration
	filter "configurations:Release"
		defines{
			"FD_RELEASE"
		}

		runtime "Release"
		optimize "on"