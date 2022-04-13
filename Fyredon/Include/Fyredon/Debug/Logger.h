// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	Logger.h: Singleton to delegate debug info on debug thread via winAPI console : Sharjeel

	Formatting adapted from:
	https://docs.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences
=============================================================================*/
#pragma once

#include <Fyredon/Other/dllport.h>

#include <windows.h>
#include <wchar.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <functional>

namespace Fyredon 
{
	class Logger {
	public:
		static Logger& getInstance();

		// Disable to prevent duplicates
		Logger(const Logger&)			= delete;
		void operator=(const Logger&)	= delete;

		// log levels
		void warn(const char* fmt, ...);
		void info(const char* fmt, ...);
		void error(const char* fmt, ...);

		// chunk text
		void chunk_start(const char* header);
		void chunk_end();
		void chunk_msg(const char* RHS, const char* fmt, ...);


	private:
		Logger();
		static Logger instance;
		bool chunk = false;
		const char* chunk_header = "";

		void message(const char* fmt, va_list args);
		
		void print_horizontal_border(bool top);
		void print_vertical_border();
		void print_vertical_border_at(int pos);
		void print_vertical_border_end();
		void print_title(const char* title);
		
		// Console members
		HANDLE h_stdout;
		DWORD mode = 0;
		CONSOLE_SCREEN_BUFFER_INFO buf_info;
		COORD size;
	};
};

// ANSI escape codes for virtual terminal sequence
#define ESC "\x1b"
#define CSI "\x1b["

// Log macros
#ifdef FD_DEBUG
	#define FD_DEBUG_INFO(fmt, ...)				Fyredon::Logger::getInstance().info(fmt, __VA_ARGS__)
	#define FD_DEBUG_WARN(fmt, ...)				Fyredon::Logger::getInstance().warn(fmt, __VA_ARGS__)
	#define FD_DEBUG_ERROR(fmt, ...)			Fyredon::Logger::getInstance().error(fmt, __VA_ARGS__)

	#define FD_DEBUG_BLOCK_BEGIN(title)			Fyredon::Logger::getInstance().chunk_start(title)
	#define FD_DEBUG_BLOCK_MSG(rhs, fmt, ...)	Fyredon::Logger::getInstance().chunk_msg(rhs, fmt, __VA_ARGS__)
	#define FD_DEBUG_BLOCK_END()				Fyredon::Logger::getInstance().chunk_end()

#else
	#define FD_DEBUG_INFO(fmt, ...)				0
	#define FD_DEBUG_WARN(fmt, ...)				0
	#define FD_DEBUG_ERROR(fmt, ...)			0

	#define FD_DEBUG_BLOCK_BEGIN(title)			0
	#define FD_DEBUG_BLOCK_MSG(rhs, fmt, ...)	0
	#define FD_DEBUG_BLOCK_END()				0
#endif