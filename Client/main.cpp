// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	main.cpp: Entry point to client editor application
=============================================================================*/

// Project Includes
#include "Fyredon/Core/Application.h"

int main() 
{
	// Get Fyredon Engine Editor Application
	Fyredon::EditorApplication& editor_app = Fyredon::EditorApplication::get_instance();

	// Initalize Editor Application
	editor_app.init(1280, 720, "Fyredon Engine Editor");

	// Exec Editor Application loop 
	editor_app.exec();

	return 0;
}