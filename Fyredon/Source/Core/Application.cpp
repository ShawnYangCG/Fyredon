// Implemnets
#include "Fyredon/Other/pch.h"
#include "Fyredon/Core/Application.h"
#include "Fyredon/Core/threads.h"
#include "Fyredon/Core/TerrainGeneration.h"

// Std Includes
#include <iostream>
#include <stdexcept>

#define GL_VERSION_MAJOR 4
#define GL_VERSION_MINOR 3

Fyredon::EditorApplication::EditorApplication(std::size_t Width, std::size_t Height, const char* FilePath)
	: window_title("Fyredon Game Engine : Editor Application"), width(Width), height(Height), project_filepath(FilePath)
{
}

// Info : Called before application loop is ran, to do inital application setup. 
void Fyredon::EditorApplication::init()
{
	// ======== Application Open GL Setup ========
	// ---- GLFW, GL Context Setup 
	setup_ogl_window_contex();
	// ---- GLEW, GL Function Loading 
	setup_ogl_loading();

	// Do Debug setup ...
	FD_DEBUG_INFO("s", "This is an info log");
	FD_DEBUG_ERROR("sd", "This is an error log", 123);
	FD_DEBUG_WARN("sf", "This is a warning log", 123.456);

	// Do FileSystem setup ...

	// Do Threading setup ...
	//ThreadManager tm = ThreadManager();
	//TerrainGenerator tg = TerrainGenerator();

	// Do GUI Setup ....
}

void Fyredon::EditorApplication::exec()
{
	// Pre Exec Setup
	while (!glfwWindowShouldClose(window))
	{
		// Do awesome stuff
		tick(); 

	}
}

// Info : All operations done within the editor application for a single tick
void Fyredon::EditorApplication::tick()
{
	paint_begin();

	// Cool stuff going on here ...
	
	paint_end();
}


// Application Painting 

// Start of Application paint operations (within Tick())
void Fyredon::EditorApplication::paint_begin()
{
	glClearColor(0.15f, 0.15f, 0.15f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// End of Application paint operations (within Tick())
void Fyredon::EditorApplication::paint_end()
{
	glfwSwapBuffers(window);
	glfwPollEvents();
}

// ============= Editor Application OpenGL Setup =============

// Info : Setup OpenGL Context via GLFW, setup Window and callbacks. 
void Fyredon::EditorApplication::setup_ogl_window_contex()
{
	if (!glfwInit())
	{
		// Logger class repalce this :)
		std::cerr << "Error::Viewer:: GLFW failed to initalize.\n";
		throw std::runtime_error("GLFW Fail"); // Need to talk about exceptions use...
	}

	// Window State
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GL_VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GL_VERSION_MINOR);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // Fixed Window Size. 
	glfwWindowHint(GLFW_SAMPLES, 4); // 4 Samples MSAA for application.
	// Create Window
	const GLFWvidmode *monitor = glfwGetVideoMode(glfwGetPrimaryMonitor());
	//width = monitor->width, height = monitor->height;
	window = glfwCreateWindow(width, height, window_title, NULL, NULL);
	if (window == NULL)
	{
		std::cerr << "Error::Viewer:: GLFW failed to initalize.\n";
		glfwTerminate();
		std::terminate();
	}

	// ============= Set GLFW Callbacks =============
	// Window Callack
	glfwSetFramebufferSizeCallback(window, &framebuffer_size_callback);
	// Mouse Callbacks
	glfwSetCursorPosCallback(window, &mouse_callback);
	glfwSetScrollCallback(window, &scroll_callback);

	// ============= Set Context and Viewport =============
	glfwMakeContextCurrent(window);
	glViewport(0, 0, width, height);
}

// Info : Setup OpenGL function loading via GLEW 
void Fyredon::EditorApplication::setup_ogl_loading()
{
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		// Logger class repalce this :)
		std::cerr << "Error::Viewer:: GLEW failed to initalize.\n";
		throw std::runtime_error("GLEW Fail"); // Need to talk about exceptions use...
	}

	// Query GL Device and Version Info - 
	render_device.vendor      = glGetString(GL_VENDOR);
	render_device.name        = glGetString(GL_RENDERER);
	render_device.version     = glGetString(GL_VERSION);
	render_device.sl_version  = glGetString(GL_SHADING_LANGUAGE_VERSION);
	// Cleanup Debug Output
	FD_DEBUG_BLOCK_BEGIN("DEBUG::OPENGL::DEVICE_INFO");
	FD_DEBUG_BLOCK_MSG("DEVICE VENDOR", "s", render_device.vendor);
	FD_DEBUG_BLOCK_MSG("DEVICE RENDERER", "s", render_device.name);
	FD_DEBUG_BLOCK_MSG("DEVICE VERSION", "s", render_device.version);
	FD_DEBUG_BLOCK_MSG("DEVICE SL VERSION", "s", render_device.sl_version);
	FD_DEBUG_BLOCK_END();

	// Need to insert each device param into debug string...

}

// ============= GLFW Callbacks Implementation =============
// 
// GLFW Callbacks 
void Fyredon::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	//
}

void Fyredon::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	// 
}

void Fyredon::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	// 
}
