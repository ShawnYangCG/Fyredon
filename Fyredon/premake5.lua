project "Fyredon"
	kind "SharedLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off" -- multithreaded DLL (MD) in VS


	targetdir("%{wks.location}/bin/" .. outdir .. "/%{prj.name}")
	objdir("%{wks.location}/bin-obj/" .. outdir .. "/%{prj.name}")
	
	files{
		"Source/Core/**.cpp",
		"Include/Fyredon/**/**.h"
	}

	includedirs{
		"%{wks.location}/Fyredon/Include/",
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