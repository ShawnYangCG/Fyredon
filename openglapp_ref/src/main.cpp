// COMP5823M - A2 : Niall Horn - main.cpp

// Project Headers
#include "viewer.h"

int main(int argc, char **argv)
{
	// Create Viewer Application Instance
	Viewer app(1024, 1024, "Niall Viewer App Reference");

	// Exec
	app.exec();

	return 0; 
}

