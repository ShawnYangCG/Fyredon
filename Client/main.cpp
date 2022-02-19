// Info : Client Application (engine editor) entry point.

// Project Includes
// Subsystem : Fyredon / Core
#include "Fyredon/Core/Example.h"
#include "Fyredon/Core/Application.h"

// Std Include 
#include<iostream>

int main() 
{
	// Create Fyredon Engine Editor Application
	Fyredon::EditorApplication app(1024, 1024, "null");

	// Initalize Editor Application
	app.init();

	// Execture Editor Application loop 
	app.exec();

	//std::cin.get();
	return 0;
}