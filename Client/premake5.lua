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
	}

	links{
		"Fyredon"
	}


	-- Windows system settings
	filter "system:windows"
		systemversion "latest"

		-- Post build step: copy DLLs generated into other project binaries
		postbuildcommands{
			("{COPY} ../bin/" .. outdir .. "/Fyredon/*.dll ../bin/" .. outdir .. "/%{prj.name}"),
			("{COPY}  %{wks.location}" ..externdir.. "/FBX/*.dll ../bin/" .. outdir .. "/%{prj.name}") -- FBX
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