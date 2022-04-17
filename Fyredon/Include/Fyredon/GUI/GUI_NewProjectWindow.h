// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	GUI_NewProjectWindow.cpp: create a new project window
=============================================================================*/
#pragma once

#include <string>

#include <Shlobj.h>
#include <io.h>

namespace Fyredon {

class GUI_NewProjectWindow {
public:
	GUI_NewProjectWindow(int windowWidth, int windowHeight);
	~GUI_NewProjectWindow();

	std::string getName();
	std::string getPath();
	void tick();
	std::string getWarning(int state);
	bool getIsNext();
	std::string TCHARTurnString(TCHAR* STR);
	LPCWSTR stringToLPCWSTR(std::string orig);
	void reLocation(int windowWidth, int windowHeight);
	bool projectExist(std::string path);

private:
	std::string Name = "";
	std::string Path = "";
	int NameState = 0;
	int PathState = 0;
	bool isNext = false;
	int windowWidth;
	int windowHeight;
	int width = 460;
	int height = 170;
};

}
