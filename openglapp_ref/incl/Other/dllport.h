// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	dllport.h: Defines DLL interface to client (import/export)
=============================================================================*/

#pragma once
#include <Other/platdetect.h>

#ifdef FD_WINDOWS

	#ifdef FD_BUILD_DLL
			#define FD_API __declspec(dllexport)
		#else
			#define FD_API __declspec(dllimport)
	#endif
#else
	#error "Only supported on windows"
#endif