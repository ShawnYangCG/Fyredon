// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	Application.h: Base Engine Editor Application : Niall, [..]
=============================================================================*/
// Project Includes
#include "Fyredon/Other/pch.h"

// Implements 
#include "Fyredon/Core/Application.h"

// GL, TBD : Move GLEW impl and incl to here.
//#define GLEW_STATIC // Fyredon DLL --> GLEW (static linking)
//#include "Fyredon/Extern/GLEW/glew.h"
//#include "Fyredon/Extern/GLFW/glfw3.h"

// Core 
#include "Fyredon/Core/threads.h"
#include "Fyredon/Core/TerrainGeneration.h"
#include "Fyredon/Core/FileSys.h"

// Debug
#include <Fyredon/Debug/Logger.h>

// GUI
#include "Fyredon/GUI/GUI_Menu.h"
#include "Fyredon/GUI/GUI_FloatingWidget.h"

// Render
#include "Fyredon/Render/Camera.h"

// Extern 
#include "dearimgui/imgui.h"
#include "dearimgui/imgui_impl_glfw.h"
#include "dearimgui/imgui_impl_opengl3.h"

// Std
#include <iostream>
#include <stdexcept>

#define GL_VERSION_MAJOR 4
#define GL_VERSION_MINOR 5
#define EDITOR_WINDOW_RESIZE
// #define EDITOR_FULLSCREEN 

// ================ Init Singleton Instance ================
Fyredon::EditorApplication Fyredon::EditorApplication::app;

Fyredon::EditorApplication& Fyredon::EditorApplication::get_instance()
{
	return app;
}

// ================ Editor Application Class Implementation ================

Fyredon::EditorApplication::~EditorApplication()
{
	if (gui_panels.size())
	{
		for (GUI_Panel* panel : gui_panels) delete panel; 
	}
}

// Info : Called before application loop is ran, to do inital application setup. 
void Fyredon::EditorApplication::init(std::size_t Width, std::size_t Height, const char* Title)
{
	// ======== Application Open GL Setup ========
	// ---- GLFW, GL Context Setup 
	setup_ogl_window_contex();

	// ---- GLEW, GL Function Loading 
	setup_ogl_loading();

	// ---- Do GUI Setup 
	setup_gui();

	// Do Debug setup ...
	FD_DEBUG_INFO("s", "This is an info log");
	FD_DEBUG_ERROR("sd", "This is an error log", 123);
	FD_DEBUG_WARN("sf", "This is a warning log", 123.456);

	// Do FileSystem setup ...

	// Do Threading setup ...
	//ThreadManager tm = ThreadManager();
	//TerrainGenerator tg = TerrainGenerator();
}

void Fyredon::EditorApplication::exec()
{
	// Pre Exec Setup

	// Edtior Loop (Game loop nested in here)
	while (!glfwWindowShouldClose(window))
	{
		// Do awesome stuff
		tick(); 
	}

	// Gui Shutdown
	shutdown_gui();
}

// Info : All operations done within the editor application for a single tick
void Fyredon::EditorApplication::tick()
{
	paint_begin();

	// Cool stuff going on here ...
	// tick engine...

	tick_gui();

	// ImGui render and forward to OpenGL Impl, todo wrap im ImGui wrapper class...
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	
	paint_end();
}


void Fyredon::EditorApplication::tick_gui()
{
	for (GUI_Panel* panel : gui_panels)
	{
		// If panel active...
		panel->tick();
	}
}


// Start of Application paint operations (within Tick())
void Fyredon::EditorApplication::paint_begin()
{
	glClearColor(0.15f, 0.15f, 0.15f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// GUI new frame, todo wrap im ImGui wrapper class...
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
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
		throw std::runtime_error("GLFW Init Fail"); // Need to talk about exceptions use...
	}

	// Window State
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GL_VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GL_VERSION_MINOR);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifndef EDITOR_WINDOW_RESIZE
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // Fixed Window Size. 
#endif
	glfwWindowHint(GLFW_SAMPLES, 4); // MSAA 4 Samples for editorapp framebuffer. 
	// Create Window
	const GLFWvidmode *monitor = glfwGetVideoMode(glfwGetPrimaryMonitor());
#ifdef EDITOR_FULLSCREEN
	width = monitor->width, height = monitor->height;
#endif
	window = glfwCreateWindow(width, height, window_title, NULL, NULL);
	if (window == NULL)
	{
		FD_DEBUG_ERROR("GLFW failed to initalize, fatal.");
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
}

GLFWwindow* Fyredon::EditorApplication::get_ogl_context()
{
	return window; 
}

// ============= GUI  =============
// 
// Info : DearImGui Setup operations
void Fyredon::EditorApplication::setup_gui()
{
	// Inital Panel Layout
	gui_panels.emplace_back(new GUI_Menu(GUI_Menu::config::EDIT_VIEW_HELP_ABOUT, 5)); // Menu bar. 
	gui_panels.emplace_back(new GUI_FloatingWidget()); // Floating Widget. 

	for (std::size_t i = 0; i < 3; ++i)
	{
		auto widg = new GUI_FloatingWidget();
		std::string tmp = "Panel " + std::to_string(i + 1);
		widg->set_title(tmp.c_str());
		gui_panels.push_back(widg);
	}
	
	// ImGui Setup 
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	// GL GUI Setup
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 450");
}

// Info : DearImGui Shutdown operations
void Fyredon::EditorApplication::shutdown_gui()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

// ============= GLFW Callbacks Implementation =============
// 
// GLFW Callbacks 
static auto EditorApp = Fyredon::EditorApplication::get_instance();

// Info : Called when window (thus window framebuffer) is resized. (width, height is in pixels) 
void Fyredon::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	EditorApp.GLFWState.width = width, EditorApp.GLFWState.height = height;
	glViewport(0, 0, width, height);
}

// Info : Called on mouse input event.
void Fyredon::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	// Set init mouse
	if (!EditorApp.GLFWState.is_init)
	{
		EditorApp.GLFWState.mousepos_prev_x = xpos;
		EditorApp.GLFWState.mousepos_prev_y = ypos;
		EditorApp.GLFWState.is_init = true;
	}
	// Mouse Offset
	EditorApp.GLFWState.mouse_offset_x = (xpos - EditorApp.GLFWState.mousepos_prev_x);
	EditorApp.GLFWState.mouse_offset_y = (ypos - EditorApp.GLFWState.mousepos_prev_y);

	// Pos
	EditorApp.GLFWState.mousepos_x = xpos;
	EditorApp.GLFWState.mousepos_y = ypos;

	// Prev Pos
	EditorApp.GLFWState.mousepos_prev_x = xpos;
	EditorApp.GLFWState.mousepos_prev_y = ypos;
}

// Info : Called on mouse scroll event. 
void Fyredon::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	EditorApp.GLFWState.scroll_y = yoffset;
}

