// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	GUI_NewProjectWindow.h: create a new project window : Shawn
=============================================================================*/
#pragma once
// Std Headers
#include <string>
#include <Shlobj.h>
#include <io.h>
// Extern Includes
#include "dearimgui/imgui.h"
namespace Fyredon
{
	enum NEWPROJECTSTATE
	{
		START,
		DEFULT
	};
	class GUI_NewProjectWindow {
	public:
		GUI_NewProjectWindow(int windowWidth, int windowHeight, int NEWPROJECTSTATE = DEFULT);
		~GUI_NewProjectWindow();
		std::string getName();
		std::string getPath();
		void tick();
		std::string getWarning(int state);
		bool getIsNext();
		bool getIsCreate();
		bool getIsClose();
		void resetState();
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
		bool CreateClick = false;
		bool CloseClick = false;
		int windowWidth;
		int windowHeight;
		int width = 460;
		int height = 170;
		int state = 0;	// 0 is for starting, 1 is for
	};
}