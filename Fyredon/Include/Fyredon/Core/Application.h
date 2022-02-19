#pragma once

// Project Includes
#include <Fyredon/Other/dllport.h>

// Extern Includes
#define GLEW_STATIC
#include "Fyredon/Extern/GLEW/glew.h"
#include "Fyredon/Extern/GLFW/glfw3.h"

// Std Headers
#include <cstddef>


namespace Fyredon 
{

/*
*   ============ Editor Application Class Info ============ 
*   Core class that defines an instance of the engine editor application to be createdand executed
*	by the client app.
*/

class FD_API EditorApplication
{
public:
	
	EditorApplication() = delete;
	EditorApplication(std::size_t Width, std::size_t Height, const char *FilePath);

	// Call from client main()
	void init();
	void exec(); 

protected:
	// OpenGL Setup
	void setup_ogl_window_contex();

	void setup_ogl_loading();

	// Application Tick
	void tick();

	void paint_begin();
	void paint_app(); // GUI Render...
	void paint_end();

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


}; // end of EditorApplication class


/*
*   ============ GLFW Input Callbacks  ============
*   Declare callbacks to be used by GLFW in response to inputs events / changes
*/
// 
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void mouse_callback(GLFWwindow* window, double xpos, double ypos);

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


}