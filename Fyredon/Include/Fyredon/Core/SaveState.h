// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	SaveState.h: Store current game state into binary file
=============================================================================*/

#pragma once

// Ext Headers 
#include "Fyredon/Debug/Logger.h"
// GLM
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace Fyredon {
	class SaveState {
	public:
		static SaveState& getInstance();

		// Disable to prevent duplicates
		SaveState(const SaveState&) = delete;
		void operator=(const SaveState&) = delete;

		void tick(glm::vec3& cam_pos, float yaw, float pitch); // updates engine state
		void load(); // loads in engine state from dat file

		bool update; // mark state to be updated
		bool loaded; // mark state to be loaded in

		// State var storag
		// Camera
		glm::vec3 cam_pos;
		float yaw;
		float pitch;
		// Lights
		// Assets

	private:
		SaveState();
		static SaveState instance;

		void encode();
		void decode();

		
	};
}