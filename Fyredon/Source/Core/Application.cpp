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
#include "Fyredon/Core/SaveState.h"

// Debug
#include <Fyredon/Debug/Logger.h>

// ECS 
#include "Fyredon/ECS/World.h"
#include "Fyredon/ECS/camera_entity.h"

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

// Extern 
#include "dearimgui/imgui.h"
#include "dearimgui/imgui_impl_glfw.h"
#include "dearimgui/imgui_impl_opengl3.h"

#pragma warning( disable : 26495) 

#define GL_VERSION_MAJOR 4
#define GL_VERSION_MINOR 5
#define EDITOR_WINDOW_RESIZE
#define MSAA_SAMPLES 12 // Just testing, lower for release ! 

#define GL_DETAILED_DEBUGGING 
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
	: dt(0.f), cur_t(0.f), prev_t(0.f), width(1024), height(1024), window_title("Editor")
{
	// ToDo : remove
	// Camera (to move to ECS world)
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
}

// Info : Called before application loop is ran, to do inital application setup. 
void Fyredon::EditorApplication::init(std::size_t Width, std::size_t Height, const char* Title)
{
	// ========= Title =========
	std::string title = Title;
	title += " :: Default Game Project";
	width = Width, height = Height, window_title = title.c_str();

	// ========= GLFW, GL Context Setup ========= 
	setup_ogl_window_contex();

	// ========= GLEW, GL Function Loading =========
	setup_ogl_loading();

	// ========= Editor GUI Setup =========
	setup_gui();

	// ========= Renderer Subsystem Setup =========
	Renderer& renderer = Renderer::get_renderer();
	renderer.init(window, MSAA_SAMPLES);

	// ========= ECS World Setup =========
	ecs_world_setup();
}

// Info : Setup default ECS world state
void Fyredon::EditorApplication::ecs_world_setup()
{
	World& world = World::get_world();

	world.init();
	world.gui_org = this->gui_org;

	// Default Cameras (Game_Cam, Scene_Cam)
	world.create_camera_entity("Scene_Camera");
	world.create_camera_entity("Game_Camera");
	assert(world.counts.camera_count == 2);

	// Setup some default test mesh here ...

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

	// Update Viewport
	glViewport(0, 0, newWidth, newHeight);

	// Update Renderer. Needs to be just viewport size based...
	Renderer::get_renderer().resize(newWidth, newHeight);
}

// Info : This should be encapsulated into its own class later for game + engine state
void Fyredon::EditorApplication::tick_engine()
{
	get_dt();

	// Tick ECS World
	World::get_world().tick(dt);
	update_inputs();

	//  ?? rm ..
	guiConfig.AppHeight = height;
	guiConfig.AppWidth = width;

	// Render
	Renderer::get_renderer().set_active_camera(&camera);
	Renderer::get_renderer().render();

	// On Play, switch to game cam
	//Renderer::get_renderer().set_active_camera(World::get_world().game_cam_entity->camera());
}

// Info : All operations done within the editor application for a single tick
void Fyredon::EditorApplication::tick_editor()
{
	paint_begin();

	tick_gui();

	paint_end();
}

// Check for play via Menu -> class bool play state ...
void Fyredon::EditorApplication::tick_play()
{
	if (((GUI_Menu*)gui_org->gui_panels["Menu"])->menu_state.play) {
		std::cout << "play" << std::endl;
	}
}

void Fyredon::EditorApplication::tick_gui()
{
	ImGui::PushStyleColor(ImGuiCol_Header,   IM_COL32(40, 40, 40, 255));
	ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(70, 70, 70, 255));
	ImGui::PushStyleColor(ImGuiCol_Button,   IM_COL32(40, 40, 40, 255));
	ImGui::PushStyleColor(ImGuiCol_TitleBg,  IM_COL32(50, 50, 50, 255));
	ImGui::PushStyleColor(ImGuiCol_FrameBg,  IM_COL32(40, 40, 40, 255));

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
	// ============= GLFW Init =============
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

	// ============= Create Window =============
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
	int yheight  = guiConfig.leftBar_height > guiConfig.rightBar_height ? guiConfig.leftBar_height : guiConfig.rightBar_height;
	int newHeight = yheight - guiConfig.yOffset;
	glViewport(0, 0, newWidth, newHeight);
}

// Info : Setup OpenGL function loading via GLEW 
void Fyredon::EditorApplication::setup_ogl_loading()
{
	// ============= GLEW Init =============
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cerr << "Error::Viewer:: GLEW failed to initalize.\n";
		throw std::runtime_error("GLEW Fail"); // Need to talk about exceptions use...
	}

	// ============= Render Device =============
	// Query GL Device and Version Info - 
	render_device.vendor     = glGetString(GL_VENDOR);
	render_device.name       = glGetString(GL_RENDERER);
	render_device.version    = glGetString(GL_VERSION);
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
{ return window;}

void Fyredon::EditorApplication::get_dt()
{
	prev_t = cur_t;
	cur_t = glfwGetTime();
	dt = cur_t - prev_t;
}

// As Camera meta state is set in application, ECS Cameras update is called from within Application

// Info : Update inputs based on GLFW State, ECS Cameras will update based on this from app singleton.
void Fyredon::EditorApplication::update_inputs()
{
	// Fetch global GLFW State
	if (GLFWState.width != 0 && GLFWState.height != 0)
		width = GLFWState.width, height = GLFWState.height;

	// Only set yaw,pitch if delta from last tick (update_camera() call). 
	GLFWState.delta_yaw   = GLFWState.mouse_offset_x != GLFWState.last_yawoffs ? GLFWState.mouse_offset_x : 0.f;
	GLFWState.delta_pitch = GLFWState.mouse_offset_y != GLFWState.last_pitchoffs ? GLFWState.mouse_offset_y : 0.f;
	GLFWState.delta_zoom  = GLFWState.scroll_y != GLFWState.last_zoom ? GLFWState.scroll_y : 0.f;
	GLFWState.cam_dt      = dt; 

	// Set last offsets
	GLFWState.last_yawoffs   = GLFWState.mouse_offset_x;
	GLFWState.last_pitchoffs = GLFWState.mouse_offset_y;
	GLFWState.last_zoom      = GLFWState.scroll_y;


	// Update Cameras in ECS world using this on next camera_comp tick() oppose to here.
	// ToDo : Below code is deprecated, remove when stable : 

	/*
	// Update Camera State for all camera components
	// Old pre ECS solution : //camera.update_camera(window, 1.f, dt, delta_yaw, delta_pitch);
	World& world = World::get_world();

	// Terrible solution currently (Have to fetch and cast camera enttity and component). Only works on camera 0.
	camera_entity* cam_ent = static_cast<camera_entity*>(world.getCameraByIndex(0));
	static_cast<camera_component*>(cam_ent->getComponentByIndex(0))->camera->update_camera(window, 1.f, dt, delta_yaw, delta_pitch);

	// Update Aspect Ratio if changed
	if (GLFWState.width && GLFWState.height)
	{
		float ar = width / height;
		camera.aspect_ratio = ar;
	} 
	*/

	// Old, Old !
	// Update Camera State
	camera.update_camera(window, 1.f, dt, GLFWState.delta_yaw, GLFWState.delta_pitch);

	// Update Aspect Ratio if changed
	if (GLFWState.width && GLFWState.height)
	{
		float ar = width / height;
		camera.aspect_ratio = 1.1f;
	}

}

// ============= ImGui GUI Setup =============
// 
// Info : DearImGui Setup operations
void Fyredon::EditorApplication::setup_gui()
{
	World* ECSworld = &World::get_world();

	// Menu bar 
	gui_org->gui_panels["Menu"] = new GUI_Menu(GUI_Menu::config::EDIT_VIEW_HELP_ABOUT, &guiConfig, 9);
	this->menuAction = new MenuAction(gui_org, &World::get_world());

	// Entity Panel
	gui_org->gui_panels["Hierarchy"] = new GUI_EntityList("Hierarchy", &guiConfig, ECSworld);

	// Component Panel
	gui_org->gui_panels["Inspector"] = new GUI_Components("Inspector", &guiConfig, ECSworld);

	// Asset Panel
	gui_org->gui_panels["Asset_Browser"] = new GUI_Asset("Asset Browser", &guiConfig, ECSworld);

	// Game Viewport
	gui_org->gui_panels["Game_Viewport"] = new GUI_Viewport("Game Viewport", &guiConfig);

	// Renderer Settings Panel 
	gui_org->gui_panels["Render_Settings"] = new GUI_RendererSettings;

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

// Info : Requires OpenGL 4.3, assume users have 4.5 spec, if an issue can revert to classic GL Error debugging. 
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
	if (type == GL_DEBUG_TYPE_ERROR)
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