// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	Application.h: Base Engine Editor Application
=============================================================================*/
#pragma once

// Project Includes
#include <Fyredon/Other/dllport.h>
#include <Fyredon/Debug/Logger.h>

// Extern Includes
#define GLEW_STATIC // Fyredon DLL --> GLEW (static linking)
#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

// Std Headers
#include <cstddef> // std::size_t

// FD
//class GLFWwindow;

namespace Fyredon 
{
	class GUI_Panel;
/*
*   ============ Editor Application Class ============
*   Core class that defines an instance of the engine editor application to be created and executed
*	by the client app.
*/

class FD_API EditorApplication
{
public:
	
	EditorApplication() = delete;
	EditorApplication(std::size_t Width, std::size_t Height, const char *FilePath);
	~EditorApplication(); 

	// Call from client main()
	void init();
	void exec(); 

protected:

	// Application Tick
	void tick();

	// OpenGL Setup
	void setup_ogl_window_contex();
	void setup_ogl_loading();

	// Application Paint Steps
	void paint_begin();
	void paint_end();

	// GUI Setup
	void setup_gui();
	void shutdown_gui();
	void tick_gui();

	// Nested Game Tick...
	// Nested Game State...

private:
	const char *window_title; 
	const char *project_filepath;

	// Application Window
	GLFWwindow* window; 
	std::size_t width, height; 

	// Render Device
	struct {
		const unsigned char* vendor;
		const unsigned char* name;
		const unsigned char* version;
		const unsigned char* sl_version;
	} render_device; 

	// GUI 
	std::vector<GUI_Panel*> gui_panels;

}; // end of EditorApplication class


//  ============ GLFW Input Callbacks ============
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void mouse_callback(GLFWwindow* window, double xpos, double ypos);

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

}