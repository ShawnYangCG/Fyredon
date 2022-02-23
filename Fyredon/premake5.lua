project "Fyredon"
	kind "SharedLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off" -- multithreaded DLL (MD) in VS


	targetdir("%{wks.location}/bin/" .. outdir .. "/%{prj.name}")
	objdir("%{wks.location}/bin-obj/" .. outdir .. "/%{prj.name}")
	
	files{
		"Source/**/**.cpp",
		"Include/Fyredon/**/**.h"
	}

	includedirs{
		"%{wks.location}/Fyredon/Include/",
		"%{wks.location}/Fyredon/Include/Fyredon/Extern/dearimgui/", -- need dearimgui dir, for its own includes #include "imgui.h" etc.
		"%{wks.location}/Fyredon/Include/Fyredon/Extern/FBX"
	}
	
	-- Linking to static libaries of GLEW and GLFW for now. Sharjeel, you can look into submodules or something better :) 
	libdirs { 
		"%{wks.location}/Extern/GLEW",
		"%{wks.location}/Extern/GLFW",
		"%{wks.location}/Extern/FBX"
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
			"FD_BUILD_DLL" -- to define the dll import/export api
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