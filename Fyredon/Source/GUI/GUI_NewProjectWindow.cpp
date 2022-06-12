// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	GUI_NewProjectWindow.cpp: create a new project window : Shawn
=============================================================================*/
#include "Fyredon/Other/pch.h"
#include "Fyredon/GUI/GUI_NewProjectWindow.h"

Fyredon::GUI_NewProjectWindow::GUI_NewProjectWindow(int windowWidth, int windowHeight, int NEWPROJECTSTATE)
{
	reLocation(windowWidth, windowHeight);
	this->state = NEWPROJECTSTATE;
}

Fyredon::GUI_NewProjectWindow::~GUI_NewProjectWindow()
{
}

std::string Fyredon::GUI_NewProjectWindow::getName()
{
	return this->Name;
}

std::string Fyredon::GUI_NewProjectWindow::getPath()
{
	return this->Path;
}

void Fyredon::GUI_NewProjectWindow::tick()
{
	// if create check pass return true else false
	ImGui::SetNextWindowPos(ImVec2(windowWidth / 2 - width/2, windowHeight / 2 - height/2));
	ImGui::SetNextWindowSize(ImVec2(width, height));
	ImGui::Begin("New Project");

	ImGui::Text("");

	ImGui::Text("Porject name:");
	ImGui::SameLine();
	static char name[MAX_PATH] = "";
	strcpy(name, this->Name.data());
	ImGui::InputText(" ", name, IM_ARRAYSIZE(name));
	this->Name = name;
	std::string waringName = getWarning(this->NameState);
	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 100, 100, 255));
	ImGui::Text(("               " + waringName).data());
	ImGui::PopStyleColor();

	ImGui::Text("Porject path:");
	ImGui::SameLine();
	static char path[MAX_PATH] = "";
	strcpy(path, this->Path.data());
	ImGui::InputText("", path, IM_ARRAYSIZE(path));
	this->Path = path;
	std::string waringPath = getWarning(this->PathState);

	ImGui::SameLine();
	if (ImGui::Button("...")) {
		TCHAR szBuffer[MAX_PATH] = { 0 };
		BROWSEINFO bi;
		ZeroMemory(&bi, sizeof(BROWSEINFO));
		bi.hwndOwner = NULL;
		bi.pszDisplayName = szBuffer;
		bi.ulFlags = BIF_RETURNFSANCESTORS;
		LPITEMIDLIST idl = SHBrowseForFolder(&bi);
		if (NULL == idl) {
		
		}
		SHGetPathFromIDList(idl, szBuffer);
		this->Path = TCHARTurnString(szBuffer);
	}

	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 100, 100, 255));
	ImGui::Text(("               " + waringPath).data());
	ImGui::PopStyleColor();

	ImGui::Text("                                              "); ImGui::SameLine();
	if (ImGui::Button("Create")) {
		if (this->Name == "")
			NameState = 1;	// check fileWaring()
		else
			NameState = 0;
		if (this->Path == "")
			PathState = 2;	// check fileWaring()
		else
			PathState = 0;
		if (NameState == 0 && PathState == 0) {
			if (projectExist(this->Path + "\\" + this->Name)) {
				NameState = 0;
				PathState = 3;
				std::cout << this->Path + "\\" + this->Name << std::endl;
			}
			else {
				if (CreateDirectoryW(stringToLPCWSTR(this->Path + "\\" + this->Name), NULL)) {
					this->isNext = true;
					this->CreateClick = true;
				}
				else
					PathState = 4;
			}
		}	
	}
	ImGui::SameLine();
	if (ImGui::Button("Close")) {
		if (state == START)
			exit(0);
		else if (state == DEFULT) {
			this->isNext = true;
			this->CloseClick = true;
		}	
	}
	ImGui::End();
}

std::string Fyredon::GUI_NewProjectWindow::getWarning(int state)
{
	if (0 == state)
		return "";
	if (1 == state)
		return "Need to input a project name";
	if (2 == state)
		return "Need to input a path for project";
	if (3 == state)
		return "Project already exist";
	if (4 == state)
		return "Path format uncorrect";
}

bool Fyredon::GUI_NewProjectWindow::getIsNext()
{
	return this->isNext;
}

bool Fyredon::GUI_NewProjectWindow::getIsCreate()
{
	return this->CreateClick;
}

bool Fyredon::GUI_NewProjectWindow::getIsClose()
{
	return this->CloseClick;
}

void Fyredon::GUI_NewProjectWindow::resetState()
{
	this->isNext = false;
	this->CreateClick = false;
	this->CloseClick = false;
	this->Name = "";
	this->Path = "";
	this->NameState = 0;
	this->PathState = 0;

}

std::string Fyredon::GUI_NewProjectWindow::TCHARTurnString(TCHAR* STR)
{
	int iLen = WideCharToMultiByte(CP_ACP, 0, STR, -1, NULL, 0, NULL, NULL);
	char* chRtn = new char[iLen * sizeof(char)];
	WideCharToMultiByte(CP_ACP, 0, STR, -1, chRtn, iLen, NULL, NULL);
	std::string str(chRtn);
	return str;
}

void Fyredon::GUI_NewProjectWindow::reLocation(int windowWidth, int windowHeight)
{
	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;
}

bool Fyredon::GUI_NewProjectWindow::projectExist(std::string path)
{
	return !(_access(path.data(), 0) == -1);
}

LPCWSTR Fyredon::GUI_NewProjectWindow::stringToLPCWSTR(std::string orig)
{
	size_t origsize = orig.length() + 1;
	const size_t newsize = 100;
	size_t convertedChars = 0;
	wchar_t* wcstring = (wchar_t*)malloc(sizeof(wchar_t) * (orig.length() - 1));
	mbstowcs_s(&convertedChars, wcstring, origsize, orig.c_str(), _TRUNCATE);

	return wcstring;
}