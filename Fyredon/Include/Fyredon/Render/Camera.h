// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	Camera.h: Camera class with 5DOF : Niall
=============================================================================*/
#pragma once

// Std Headers
#include <sstream>

// Ext Headers 
// GLM
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

// FDs
struct GLFWwindow;

namespace Fyredon
{
// Info : Basic Camera class using GLFW for input polling
class Camera
{
public:
	Camera(glm::vec3 Pos, float Fov, float Aspect, bool Freelook);
	Camera() {}
	~Camera() = default; 

	// View Members
	glm::vec3 position;
	glm::vec3 ref_up;

	// Camera Basis Members
	glm::vec3 basis_x, basis_y, basis_z;

	// Mouse Rotation Members - 
	float yaw, pitch;
	float yaw_min, yaw_max;
	float pitch_min, pitch_max;

	// Camera Data
	float sensitvity;
	float fov, aspect_ratio, near_plane, far_plane;
	bool free_look;

public:
	glm::mat4 get_view_matrix();
	glm::mat4 get_proj_matrix();

	void update_camera(GLFWwindow *window, float Camera_Speed, float dt, float yaw, float pitch);

	std::ostringstream debug();
	void calc_basis();
	
};
}

