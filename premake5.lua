workspace "Fyredon"
	architecture "x64"

	configurations{
		"Debug",
		"Release"
	}

		
outdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
externdir = "Extern"

include "Fyredon"
include "Client"