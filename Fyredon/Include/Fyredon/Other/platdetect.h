// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	platdetect.h: Detects the current platform, only supports Win64 for now
=============================================================================*/
#pragma once

#ifdef _WIN32

	#ifdef _WIN64									// 64-bit
		#define FD_WINDOWS

		#else										// 32-bit
		#error "No support for 32-bit systems"
	#endif

#endif


// Add other platforms below if wish to support more platforms in later versions
