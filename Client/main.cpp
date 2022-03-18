// Info : Client Application (engine editor) entry point.

// Project Includes
// Subsystem : Fyredon / Core
#include "Fyredon/Core/Application.h"


int main() 
{
	// Create Fyredon Engine Editor Application
	Fyredon::EditorApplication app(1024, 1024, "null");

	// Initalize Editor Application
	app.init();

	// Execture Editor Application loop 
	app.exec();

	return 0;
}