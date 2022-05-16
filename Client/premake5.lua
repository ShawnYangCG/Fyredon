	project "Client"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"


	targetdir("%{wks.location}/bin/" .. outdir .. "/%{prj.name}")
	objdir("%{wks.location}/bin-obj/" .. outdir .. "/%{prj.name}")

	files{
		"**.cpp"
	}

	includedirs{
		"%{wks.location}/Fyredon/Include/",
		"%{wks.location}/Extern/Include/",
		"%{wks.location}/Extern/Include/FBX/"        -- need abs fbx dir, for cases of : #include "fbxsdk/fbxsdk/fbxsdk_version.h"
	}
	
	libdirs { 
		"%{wks.location}/Extern/Binary/FBX/"
	}
	
	links{
		"libfbxsdk.lib",
		"Fyredon"
	}


	-- Windows system settings
	filter "system:windows"
		systemversion "latest"

		-- Post build step: copy DLLs generated into other project binaries
		postbuildcommands{
			("{COPY} ../bin/" .. outdir .. "/Fyredon/*.dll ../bin/" .. outdir .. "/%{prj.name}"),
			("{COPY}  %{wks.location}" ..externdir.. "/Binary/FBX/*.dll ../bin/" .. outdir .. "/%{prj.name}") -- FBX
		}


	-- Debug configuration
	filter "configurations:Debug"
		defines{
			"FD_DEBUG",
			"FBXSDK_SHARED"
		}
		runtime "Debug"
		symbols "on"


	-- Release configuration
	filter "configurations:Release"
		defines{
			"FD_RELEASE",
			"FBXSDK_SHARED"
		}

		runtime "Release"
		optimize "on"