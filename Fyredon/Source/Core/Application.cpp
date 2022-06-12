// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	Application.h: Base Engine Editor Application : Niall, Shawn
=============================================================================*/
#include "Fyredon/Other/pch.h"
// Implements 
#include "Fyredon/Core/Application.h"

// Core 
#include "Fyredon/Core/Threads.h"
#include "Fyredon/Core/FileSystem.h"
#include "Fyredon/Core/GameWorld.h"
#include "Fyredon/Core/SaveState.h"

// Debug
#include <Fyredon/Debug/Logger.h>

// GUI
#include "Fyredon/GUI/GUI_Menu.h"
#include "Fyredon/GUI/GUI_FloatingWidget.h"
#include "Fyredon/GUI/GUI_RendererSettings.h"
#include "Fyredon/GUI/GUI_EntityList.h"
#include "Fyredon/GUI/GUI_Components.h"
#include "Fyredon/GUI/GUI_GameWorld.h"
#include "Fyredon/GUI/GUI_Asset.h"

// Render
#include "Fyredon/Render/Camera.h"
#include "Fyredon/Render/Texture.h"
#include "Fyredon/Render/Renderer.h"

/*
// Script
#include "Fyredon/Script/LuaVM.h"
#include <stdio.h>
#include <direct.h>*/

// Extern 
#include "dearimgui/imgui.h"
#include "dearimgui/imgui_impl_glfw.h"
#include "dearimgui/imgui_impl_opengl3.h"

#pragma warning( disable : 26495) 

#define GL_VERSION_MAJOR 4
#define GL_VERSION_MINOR 5
#define EDITOR_WINDOW_RESIZE
#define MSAA_SAMPLES 12 // Just testing, lower for release ! 

//#define GL_DETAILED_DEBUGGING 
#define GL_DETAILED_ERROR_CHECK

//#define EDITOR_FULLSCREEN 
//#define ENABLE_CREATE_NEW_PROJECT

// ================ Init Singleton Instance + Static Data ================
Fyredon::EditorApplication Fyredon::EditorApplication::app;
decltype(Fyredon::EditorApplication::GLFWState) Fyredon::EditorApplication::GLFWState;

Fyredon::EditorApplication& Fyredon::EditorApplication::get_instance()
{
	return app;
}

// ================ Editor Application Class Implementation ================

Fyredon::EditorApplication::EditorApplication()
	: dt(0.f), cur_t(0.f), prev_t(0.f), width(1024), height(1024), last_yawoffs(0.f),
	last_pitchoffs(0.f), last_zoom(0.f), window_title("null")
{
	// Camera (to move to world)
	camera = Camera(glm::vec3(3.f, 1.f, -1.f), 80.f, RENDER_RES_X / RENDER_RES_Y, true);

	gui_org = new GUI_PanelOrgnizer();
}

// Static Members
bool Fyredon::EditorApplication::isSizeChange = true; 
GUIConfig Fyredon::EditorApplication::guiConfig = GUIConfig();

Fyredon::EditorApplication::~EditorApplication()
{
	glfwDestroyWindow(window); window = nullptr;
	glfwTerminate();
	/*
	if (gui_org) {
		gui_org->~GUI_PanelOrgnizer();
		free(gui_org);
	}
	
	if (this->menuAction) {
		this->menuAction->~MenuAction();
		free(this->menuAction);
	}

	if (this->ECSworld) {
		this->ECSworld->~World();
		free(this->ECSworld);
	}

	if (this->renderSys) {
		this->renderSys->~EngineRenderSystem();
		free(this->renderSys);
	}
	*/
}

// Info : Called before application loop is ran, to do inital application setup. 
void Fyredon::EditorApplication::init(std::size_t Width, std::size_t Height, const char* Title)
{
	// Override defaults
	std::string title = Title;
	title += " :: Default Game Project";
	width = Width, height = Height, window_title = title.c_str();

	// ======== setup a ECS world ======== 
	this->ECSworld = new World(this->gui_org);

	// ======== Application Open GL Setup ========
	// ---- GLFW, GL Context Setup 
	setup_ogl_window_contex();

	// ---- GLEW, GL Function Loading 
	setup_ogl_loading();

	// ---- Editor GUI Setup 
	setup_gui();

	// ToDo: A Susbsystem manager class, for now just create renderer instance within EditorApp
	// ---- Renderer Subsystem Setup
	Renderer& renderer = Renderer::get_renderer();
	renderer.init(window, MSAA_SAMPLES);

	// Test terrain (bypass ECS)
	//GameWorld::get_world().create_terrain();

	// ========== setup system ===============
	// render system
	this->renderSys = new EngineRenderSystem(this->ECSworld, &renderer);

	// set a default camera
	//camera_entity* camEntity = new camera_entity();
	//this->ECSworld->addCamera(camEntity);

	/*
	// Script test (demo, should be removed)
	Fyredon::LuaVM luaVM;
	luaVM.LoadScript("script/test.lua");
	int result = luaVM.Call<int, int, int>("test_func", 3, 2);
	std::cout << "test_func return: " << result << std::endl;
	//system("pause");*/
}

std::string Fyredon::EditorApplication::create_new_project()
{
	bool isNext = false;
	GUI_NewProjectWindow* newProject = new GUI_NewProjectWindow(width, height);
	while (!glfwWindowShouldClose(window)) {
		if (isNext)
			break;
		if (isSizeChange == true) {
			if (GLFWState.width != 0 && GLFWState.height != 0) {
				width = GLFWState.width, height = GLFWState.height;
				newProject->reLocation(width, height);
			}
			isSizeChange = false;
		}

		paint_begin();

		newProject->tick();
		isNext = newProject->getIsNext();

		paint_end();
	}
	std::string projectName = newProject->getName();
	std::string projectPath = newProject->getPath();
	return projectPath + "\\" + projectName;
}

// Info : Executes the Editor Application untill its window is closed
void Fyredon::EditorApplication::exec()
{
	// Pre Exec Setup
	// create new project
#ifdef ENABLE_CREATE_NEW_PROJECT
	create_new_project();
#endif

	// Edtior Loop (Game loop nested in here)
	while (!glfwWindowShouldClose(window))
	{
		tick_engine();
		tick_editor();
		
		// If Play ...
		tick_play();
	}

	// Post Exec Shutdown
	shutdown_gui();
}

void Fyredon::EditorApplication::resize()
{
	std::map<std::string, GUI_Panel*>::iterator iter;
	for (iter = gui_org->gui_panels.begin(); iter != gui_org->gui_panels.end(); iter++) {
		if (iter->second) iter->second->resize();
	}

	int newWidth = guiConfig.AppWidth - guiConfig.leftBar_width - guiConfig.rightBar_width - guiConfig.xOffset;
	int yheight = guiConfig.leftBar_height > guiConfig.rightBar_height ?
		guiConfig.leftBar_height : guiConfig.rightBar_height;
	int newHeight = yheight - guiConfig.yOffset;
	glViewport(0, 0, newWidth, newHeight);
	Renderer::get_renderer().resize(newWidth, newHeight);
}

// Info : This should be encapsulated into its own class later for game + engine state
void Fyredon::EditorApplication::tick_engine()
{
	get_dt();

	// ToDo : Put into engine class
	// Tick World when game run
	GameWorld::get_world().tick(dt);
	update_camera();

	guiConfig.AppHeight = height;
	guiConfig.AppWidth = width;

	// submit mesh from world to renderer
	GUI_RendererSettings* Rset = (GUI_RendererSettings*)gui_org->gui_panels["setting"];
	// the render prims map haven't finished so use this for testing, need to remove next 2 lines
	this->renderSys->setCamera(&camera);
	this->renderSys->setLight(&Rset->state.light_pos, &Rset->state.light_strength);
	this->renderSys->tick();

	// Tick Renderer
	Renderer::get_renderer().game_cam = &this->camera; 
	Renderer::get_renderer().render();

	// Tick state update (if save/load state requested)
	if (SaveState::getInstance().update) {			// -- State saved
		SaveState::getInstance().tick(camera.position, camera.yaw, camera.pitch);
	}
	if (SaveState::getInstance().loaded) {			// -- State loaded
		SaveState::getInstance().load();
		// Inject state
		// Camera state
		camera.position = SaveState::getInstance().cam_pos;
		camera.yaw = SaveState::getInstance().yaw;
		camera.pitch = SaveState::getInstance().pitch;
		camera.calc_basis();
		// 
	}
}

void Fyredon::EditorApplication::tick_play()
{
	if (((GUI_Menu*)gui_org->gui_panels["Menu"])->menu_state.play) {
		//================== play ==================
		this->ECSworld->init();
		//================== play ==================
		this->ECSworld->play();
	}
	else {
		if(!this->ECSworld->isStop())
			this->ECSworld->stop();
	}

	if (((GUI_Menu*)gui_org->gui_panels["Menu"])->menu_state.reset) {
		this->ECSworld->reset();

		((GUI_Menu*)gui_org->gui_panels["Menu"])->menu_state.reset = false;
	}

	//============ only for test =================

	//============================================
}

// Info : All operations done within the editor application for a single tick
void Fyredon::EditorApplication::tick_editor()
{
	paint_begin();

	tick_gui();
	this->ECSworld->tick();

	paint_end();
}

void Fyredon::EditorApplication::tick_gui()
{
	ImGui::PushStyleColor(ImGuiCol_Header, IM_COL32(40, 40, 40, 255));
	ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(70, 70, 70, 255));
	ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(40, 40, 40, 255));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, IM_COL32(50, 50, 50, 255));
	ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(40, 40, 40, 255));

	if (isSizeChange == true) {
		resize();
		isSizeChange = false;
	}

	std::map<std::string, GUI_Panel*>::iterator iter;
	for (iter = gui_org->gui_panels.begin(); iter != gui_org->gui_panels.end(); iter++) {
		if (iter->second) iter->second->tick();
	}

	// GUI's action
	this->menuAction->tick();

	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();

	// tick system
}

// Start of Application paint operations (within Tick())
void Fyredon::EditorApplication::paint_begin()
{
	glClearColor(0.15f, 0.15f, 0.15f, 1.0);
	glDisable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT);

	// Set GUI Dims
	guiConfig.AppHeight = height;
	guiConfig.AppWidth = width;

	// GUI new frame, todo wrap im ImGui wrapper class...
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

// End of Application paint operations (within Tick())
void Fyredon::EditorApplication::paint_end()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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
	glfwWindowHint(GLFW_SAMPLES, MSAA_SAMPLES);
#ifdef EDITOR_WINDOW_RESIZE
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
#else
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
#endif

	// Create Window
	GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* monitor = glfwGetVideoMode(primaryMonitor);
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
	int newWidth = guiConfig.AppWidth - guiConfig.leftBar_width - guiConfig.rightBar_width - guiConfig.xOffset;
	int yheight = guiConfig.leftBar_height > guiConfig.rightBar_height ?
		guiConfig.leftBar_height : guiConfig.rightBar_height;
	int newHeight = yheight - guiConfig.yOffset;
	glViewport(0, 0, newWidth, newHeight);
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
	render_device.vendor = glGetString(GL_VENDOR);
	render_device.name = glGetString(GL_RENDERER);
	render_device.version = glGetString(GL_VERSION);
	render_device.sl_version = glGetString(GL_SHADING_LANGUAGE_VERSION);

	// Cleanup Debug Output
	FD_DEBUG_BLOCK_BEGIN("DEBUG::OPENGL::DEVICE_INFO");
	FD_DEBUG_BLOCK_MSG("DEVICE VENDOR", "s", render_device.vendor);
	FD_DEBUG_BLOCK_MSG("DEVICE RENDERER", "s", render_device.name);
	FD_DEBUG_BLOCK_MSG("DEVICE VERSION", "s", render_device.version);
	FD_DEBUG_BLOCK_MSG("DEVICE SL VERSION", "s", render_device.sl_version);
	FD_DEBUG_BLOCK_END();

	// ============= Setup Debug Messages =============
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(message_callback, 0);

}

GLFWwindow* Fyredon::EditorApplication::get_ogl_context()
{
	return window;
}

void Fyredon::EditorApplication::get_dt()
{
	prev_t = cur_t;
	cur_t = glfwGetTime();
	dt = cur_t - prev_t;
}

void Fyredon::EditorApplication::update_camera()
{
	// Fetch global GLFW State
	if (GLFWState.width != 0 && GLFWState.height != 0)
		width = GLFWState.width, height = GLFWState.height;

	// Only set yaw,pitch if delta from last tick (update_camera() call). 
	float delta_yaw = GLFWState.mouse_offset_x != last_yawoffs ? GLFWState.mouse_offset_x : 0.f;
	float delta_pitch = GLFWState.mouse_offset_y != last_pitchoffs ? GLFWState.mouse_offset_y : 0.f;
	float delta_zoom = GLFWState.scroll_y != last_zoom ? GLFWState.scroll_y : 0.f;
	// Set last offsets
	last_yawoffs = GLFWState.mouse_offset_x;
	last_pitchoffs = GLFWState.mouse_offset_y;
	last_zoom = GLFWState.scroll_y;

	// Update Camera State
	camera.update_camera(window, 1.f, dt, delta_yaw, delta_pitch);

	// Update Aspect Ratio if changed
	if (GLFWState.width && GLFWState.height)
	{
		float ar = width / height;
		camera.aspect_ratio = ar;
	}
}

// ============= ImGui GUI Setup =============
// 
// Info : DearImGui Setup operations
void Fyredon::EditorApplication::setup_gui()
{
	// Menu bar 
	gui_org->gui_panels["Menu"] = new GUI_Menu(GUI_Menu::config::EDIT_VIEW_HELP_ABOUT, &guiConfig, 9);
	this->menuAction = new MenuAction(this->gui_org, this->ECSworld);

	// Entity Panel
	gui_org->gui_panels["Hierarchy"] = new GUI_EntityList("Hierarchy", &guiConfig, this->ECSworld);

	// Component Panel
	gui_org->gui_panels["Inspector"] = new GUI_Components("Inspector", &guiConfig, this->ECSworld);

	// Asset Panel
	gui_org->gui_panels["Asset Browser"] = new GUI_Asset("Asset Browser", &guiConfig, this->ECSworld);

	// game viewport
	gui_org->gui_panels["Game Viewport"] = new GUI_Viewport("Game Viewport", &guiConfig);

	// Standins
	//gui_panels.emplace_back(new GUI_FloatingWidget("Floating Panel"));

	// Testing only
	//gui_panels.emplace_back(new GUI_GameWorld);

	// Renderer Settings Panel 
	gui_org->gui_panels["setting"] = new GUI_RendererSettings;

	// ImGui Context Setup
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();

	// ImGui Impelementation Setup
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

// ============= GL DEBUG Callbacks Implementation =============

void Fyredon::message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const char* msg, const void* userParm)
{
#ifdef GL_DETAILED_ERROR_CHECK
	if (type == GL_DEBUG_TYPE_ERROR)
	{
		std::cerr << "GL_ERROR:: " << "type: " << type << " severity: " << severity << " message: " << msg << std::endl;
		return; 
	}
#endif
#ifdef GL_DETAILED_DEBUGGING
	if (type != GL_DEBUG_TYPE_ERROR)
	{
		std::cerr << "GL_INFO:: " << "type: " << type << " severity: " << severity << " message: " << msg << std::endl;
	}
#endif
}

// ============= GLFW Callbacks Implementation =============
// GLFW Callbacks 
static auto EditorApp = Fyredon::EditorApplication::get_instance();

// Info : Called when window (thus window framebuffer) is resized (units is pixels) 
void Fyredon::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	EditorApp.GLFWState.width = width, EditorApp.GLFWState.height = height;
	glViewport(0, 0, width, height);
	EditorApplication::get_instance().isSizeChange = true;
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