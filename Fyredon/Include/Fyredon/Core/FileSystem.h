// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	FileSys.h: Asynchronous file reading/writing : Sharjeel
=============================================================================*/
#pragma once

#include <Fyredon/Other/dllport.h>
#include <Fyredon/Debug/Logger.h>
#include <filesystem>

namespace Fyredon {

	class FileSys {
	public:
		static FileSys& getInstance();

		// Disable to prevent duplicates
		FileSys(const FileSys&) = delete;
		void operator=(const FileSys&) = delete;

		void mount(const char* vpath, const char* ppath);
		void unmount(const char* path);
		bool check_path(const std::string& path, std::string& ppath);

		char* read(const std::string& path);


	private:
		FileSys();
		static FileSys instance;

		// Map of virtual & physical mount locations
		std::unordered_map<std::string, std::vector<std::string>> mount_points;

		// Helpers
		void tokenize(const std::string& str, const char delim, std::vector<std::string>& out);

	};

}

#define FD_ROOT (std::filesystem::current_path() / "..\\Fyredon\\").u8string()