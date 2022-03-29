project "Render"
	kind "SharedLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off" -- multithreaded DLL (MD) in VS


	targetdir("%{wks.location}/bin/" .. outdir .. "/%{prj.name}")
	objdir("%{wks.location}/bin-obj/" .. outdir .. "/%{prj.name}")
	
	files{
		"Source/**/**.cpp",
		"Include/**/**.h"
	}

	includedirs{
		"%{wks.location}/Render/Include/",
		"%{wks.location}/Extern/Include/"
	}
	
	-- Linking to static libaries of GLEW and GLFW to the RenderSubsystem shared libary. 
	libdirs { 
		"%{wks.location}/Extern/Binary/GLEW/",
		"%{wks.location}/Extern/Binary/GLFW/",
	}
	
	links{
		"glew32s.lib",
		"glfw3.lib",
		"opengl32.lib",
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