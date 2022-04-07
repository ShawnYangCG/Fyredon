// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	Application.h: Base Engine Editor Application : Niall, [..]
=============================================================================*/
// Project Includes
#include "Fyredon/Other/pch.h"

// Implements 
#include "Fyredon/Core/Application.h"

/* GL, TBD : Move GLEW impland incl to here.
#define GLEW_STATIC 
#include "Fyredon/Extern/GLEW/glew.h"
#include "Fyredon/Extern/GLFW/glfw3.h" */

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
#include "Fyredon/Render/Texture.h"

// Extern 
#include "dearimgui/imgui.h"
#include "dearimgui/imgui_impl_glfw.h"
#include "dearimgui/imgui_impl_opengl3.h"

// Std
#include <iostream>
#include <stdexcept>

#pragma warning( disable : 26495) 

#define GL_VERSION_MAJOR 4
#define GL_VERSION_MINOR 5
#define EDITOR_WINDOW_RESIZE
// #define EDITOR_FULLSCREEN 

// ================ Init Singleton Instance + Static Data ================
Fyredon::EditorApplication Fyredon::EditorApplication::app;
decltype(Fyredon::EditorApplication::GLFWState) Fyredon::EditorApplication::GLFWState;

Fyredon::EditorApplication& Fyredon::EditorApplication::get_instance()
{
	return app;
}

// ================ Editor Application Class Implementation ================

Fyredon::EditorApplication::EditorApplication() 
	: dt(0.f), cur_t(0.f), prev_t(0.f), width(512), height(512), last_yawoffs(0.f),
	last_pitchoffs(0.f), last_zoom(0.f), window_title("null")
{
	camera = Camera(glm::vec3(3.f, 1.f, -1.f), 80.f, width / height, true);
}

Fyredon::EditorApplication::~EditorApplication()
{
	if (gui_panels.size())
	{
		for (GUI_Panel* panel : gui_panels) delete panel; 
	}

	glfwDestroyWindow(window); window = nullptr;
	glfwTerminate();
}

// Info : Called before application loop is ran, to do inital application setup. 
void Fyredon::EditorApplication::init(std::size_t Width, std::size_t Height, const char* Title)
{
	// Override defaults
	width = Width, height = Height, window_title = Title; 
	// ======== Application Open GL Setup ========
	// ---- GLFW, GL Context Setup 
	setup_ogl_window_contex();

	// ---- GLEW, GL Function Loading 
	setup_ogl_loading();

	// ---- Editor GUI Setup 
	setup_gui();

	// ---- RendererSS Setup
	renderer = new Renderer(window);
	viewport_tex = nullptr; // Retrive from renderer

	// Do Debug setup ...
	FD_DEBUG_INFO("s", "This is an info log");
	FD_DEBUG_ERROR("sd", "This is an error log", 123);
	FD_DEBUG_WARN("sf", "This is a warning log", 123.456);

	// We need a Susbsystem manager class, for now just create within EditorApp
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
		tick_engine();
		tick_editor();
	}

	// Shutdown
	shutdown_gui();
}

// Info : This should be encapsulated into its own class later for game + engine state
void Fyredon::EditorApplication::tick_engine()
{
	get_dt();

	// Tick World
	// ...

	update_camera();

	// Tick Renderer
	renderer->render(camera);
	// Retrive Viewport Texture
}

// Info : All operations done within the editor application for a single tick
void Fyredon::EditorApplication::tick_editor()
{
	paint_begin();

	tick_gui();
	
	paint_end();
}


void Fyredon::EditorApplication::tick_gui()
{
	for (GUI_Panel* panel : gui_panels)
	{
		// If panel active...
		panel->tick();
	}

	// ImGui render and forward to OpenGL Impl, todo wrap im ImGui wrapper class...
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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
	glfwWindowHint(GLFW_SAMPLES, 4); // MSAA 4 Samples for editorapp framebuffer. 
	#ifndef EDITOR_WINDOW_RESIZE
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // Fixed Window Size. 
	#endif

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
 
void Fyredon::EditorApplication::get_dt()
{
	prev_t = cur_t;
	cur_t  = glfwGetTime();
	dt = cur_t - prev_t;
}

void Fyredon::EditorApplication::update_camera()
{
	// Fetch global GLFW State
	width = GLFWState.width, height = GLFWState.height;

	// Only set yaw,pitch if delta from last tick (update_camera() call). 
	float delta_yaw   = GLFWState.mouse_offset_x != last_yawoffs   ? GLFWState.mouse_offset_x : 0.f;
	float delta_pitch = GLFWState.mouse_offset_y != last_pitchoffs ? GLFWState.mouse_offset_y : 0.f;
	float delta_zoom  = GLFWState.scroll_y       != last_zoom      ? GLFWState.scroll_y       : 0.f;
	// Set last offsets
	last_yawoffs   = GLFWState.mouse_offset_x;
	last_pitchoffs = GLFWState.mouse_offset_y;
	last_zoom      = GLFWState.scroll_y;

	// Update Camera State
	camera.update_camera(window, 1.f, dt, delta_yaw, delta_pitch);

	// Update Aspect Ratio if changed
	if (GLFWState.width != 0 && GLFWState.height != 0)
	{
		float ar = width / height;
		camera.aspect_ratio = ar;
	}
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

// Info : DearImGui Shutdown operations (move to imgui wrapper class)
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

