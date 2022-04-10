// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	FileSys.cpp: Asynchronous file reading/writing : Sharjeel
=============================================================================*/
#include "Fyredon/Other/pch.h"
// Implements
#include <Fyredon/Core/FileSystem.h>

/*
	TODO
	=====
	- Async read
	- Packaged files read (pak)
*/

// Initialize once
Fyredon::FileSys Fyredon::FileSys::instance;

Fyredon::FileSys::FileSys() {}

void CALLBACK FileIOCompletionRoutine(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped){}

// Get the singleton instance of the filesys class (intended to be thread-safe)
Fyredon::FileSys& Fyredon::FileSys::getInstance()
{
	return instance;
}

bool Fyredon::FileSys::check_path(const std::string& path, std::string& ppath)
{
	if (path[0] != '/') {
		// In project directory
		ppath = path;
		
		DWORD attribs = GetFileAttributes(std::wstring(path.begin(), path.end()).c_str());
		return !(attribs == INVALID_FILE_ATTRIBUTES && GetLastError() == ERROR_FILE_NOT_FOUND);
		
	}


	std::vector<std::string> dirs;
	tokenize(path, '/', dirs);
	const std::string& vdir = dirs.front();

	// Was not mounted
	if (mount_points.find(vdir) == mount_points.end()) {
		return false;
	}

	std::string rest = path.substr(vdir.size() + 1, path.size() - vdir.size());

	// File exists in one of the mounted vdirs
	for (const std::string& p : mount_points[vdir]) {
		std::string curr_path = p + rest;
		FD_DEBUG_WARN("s", curr_path.c_str());
		DWORD attribs = GetFileAttributes(std::wstring(curr_path.begin(), curr_path.end()).c_str());
		if (!(attribs == INVALID_FILE_ATTRIBUTES && GetLastError() == ERROR_FILE_NOT_FOUND)) {
			ppath = path;
			return true;
		}
	}

	return false;
}

// Mount virtual directory to physical directory
void Fyredon::FileSys::mount(const char* vpath, const char* ppath)
{
	mount_points[vpath].push_back(ppath);
}

// Remove virtual directories
void Fyredon::FileSys::unmount(const char* path)
{
	mount_points[path].clear();
}

// Read byte file
char* Fyredon::FileSys::read(const std::string& path)
{
	char* buffer;
	std::string ppath;
	bool exists = check_path(path, ppath);

	if (exists) {
		HANDLE file = CreateFile(
			std::wstring(path.begin(), path.end()).c_str(),
			GENERIC_READ,
			FILE_SHARE_READ,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
			NULL
		);

		LARGE_INTEGER file_size;
		GetFileSizeEx(file, &file_size);
		auto size = file_size.QuadPart;
		FD_DEBUG_INFO("sd", "File size is: ", (int) size);
		buffer = new char[size];

		OVERLAPPED o = { 0 };
		bool res = ReadFileEx(file, buffer, size, &o, FileIOCompletionRoutine);
		CloseHandle(file);

		if (!res) {
			FD_DEBUG_ERROR("sss", "File ", path.c_str(), " could not be read!");
			return nullptr;
		}

		return buffer;
	}
	FD_DEBUG_ERROR("sss", "File ", path.c_str(), " does not exist");
	return nullptr;
}

/* HELPERS */
void Fyredon::FileSys::tokenize(const std::string& str, const char delim, std::vector<std::string>& out)
{
	size_t start;
	size_t end = 0;
	while ((start = str.find_first_not_of(delim, end)) != std::string::npos) {
		end = str.find(delim, start);
		out.push_back(str.substr(start, end - start));
	}
}