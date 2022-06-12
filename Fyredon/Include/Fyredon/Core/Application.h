// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	Application.h: Base Engine Editor Application : Niall
=============================================================================*/
#pragma once

// Project Includes
#include <Fyredon/Other/dllport.h>
#include "Fyredon/Render/Camera.h"
#include "Fyredon/Render/Renderer.h"
#include "Fyredon/ECS/World.h"
#include "Fyredon/GUI/GUIConfig.h"
#include "Fyredon/GUI/GUI_PanelOrgnizer.h"
#include "Fyredon/GUI/GUI_NewProjectWindow.h"
#include "Fyredon/Action/MenuAction.h"
#include "Fyredon/ECS/System/EngineRenderSystem.h"
#include "Fyredon/GUI/GUI_Viewport.h"
#include "Fyredon/ECS/camera_entity.h"

// Extern Includes
#include "GLEW/glew.h"
#include "GLFW/glfw3.h"

// Std Headers
#include <vector>
#include <cstddef> // std::size_t

namespace Fyredon {

	class GUI_Panel;
	class Texture;
	class GameWorld;
	class Renderer;
	class SaveState;
	/*
	*   ============ Editor Application Class ============
	*   Core class that defines an instance of the engine editor application to be created and executed
	*	by the client app.
	*/

	class FD_API EditorApplication
	{
	public:
		~EditorApplication();

		static EditorApplication& get_instance();

		// Call from client main()
		void init(std::size_t Width, std::size_t Height, const char* Title);
		void exec();
		void resize();

		// GLFW / Input State (modified via public callback access)
		static struct {
			int    width, height;
			double mouse_offset_x = 0.f, mouse_offset_y = 0.f;
			double mousepos_x = 0.f, mousepos_y = 0.f;
			double mousepos_prev_x = 0.f, mousepos_prev_y = 0.f;
			double scroll_y = 0.f;
			bool is_init = false;
		} GLFWState;

		static bool isSizeChange;
		static GUIConfig guiConfig;

	protected:
		EditorApplication();

		// create new project
		std::string create_new_project();

		// Application Tick
		void tick_editor();

		// Engine / Game Tick
		void tick_engine();

		// play
		void tick_play();

		// OpenGL Setup
		void setup_ogl_window_contex();
		void setup_ogl_loading();
		GLFWwindow* get_ogl_context();

		// Application Paint Steps
		void paint_begin();
		void paint_end();

		// GUI Setup
		void setup_gui();
		void shutdown_gui();
		void tick_gui();
		void viewport_gui();

		// Engine/World (temp)
		void update_camera();
		void get_dt();

	private:
		static EditorApplication app; // singleton

		const char* window_title;
		const char* project_filepath;

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
		GUI_PanelOrgnizer* gui_org;
		MenuAction* menuAction;

		// Camera 
		Camera camera;
		float last_yawoffs, last_pitchoffs, last_zoom;

		// Editor State
		float dt;
		float cur_t, prev_t;

		// Game World
		//GameWorld* world;
		World* ECSworld;
		EngineRenderSystem* renderSys;

	}; // EditorApp

	//  ============ GL Debug Callbacks Requires (4.3) ============
	void GLAPIENTRY message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const char* msg, const void* userParm);

	//  ============ GLFW Input Callbacks ============
	void framebuffer_size_callback(GLFWwindow* window, int width, int height);

	void mouse_callback(GLFWwindow* window, double xpos, double ypos);

	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


}