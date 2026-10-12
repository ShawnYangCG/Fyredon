// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	Camera.cpp: Camera Class Impelementation 
=============================================================================*/
#include "Fyredon/Other/pch.h"
// Implments
#include "Fyredon/Render/Camera.h"

// Ext Headers
#include "GLFW/glfw3.h"

// Std Headers (use PCH)
//#include <iostream>
//#include <algorithm>

// =================== Camera Implementation ===================

Fyredon::Camera::Camera(glm::vec3 Pos, float Fov, float Aspect, bool Freelook) 
	: position(Pos), fov(Fov), aspect_ratio(Aspect), free_look(Freelook), ref_up(glm::vec3(0.f, 1.f, 0.f))
{
	// Init Camera Bases
	basis_z = glm::normalize(glm::vec3(0.f, 0.f, 1.f)); // Faces out screen (not tgt vector).
	basis_x = glm::normalize(glm::cross(ref_up, basis_z));
	basis_y = glm::normalize(glm::cross(basis_z, basis_x));


	yaw = 0.0f, pitch = 0.0f, sensitvity = 0.5f;
	yaw_min = -120.0f,  yaw_max = 120.0f;
	pitch_min = -89.0f, pitch_max = 89.0f;
	if (free_look) calc_basis();

	near_plane = 0.01f;
	far_plane  = 100.0f;
}

glm::mat4 Fyredon::Camera::get_view_matrix()
{
	// flip z basis for target
	return glm::lookAt(position, (position - basis_z), ref_up);
}

glm::mat4 Fyredon::Camera::get_proj_matrix()
{
	auto projection = glm::perspective(glm::radians(fov), aspect_ratio, near_plane, far_plane);
	projection[0][0] /= viewport_scale;
	projection[1][1] /= viewport_scale;
	return projection;
}

void Fyredon::Camera::update_camera(float deltaX, float deltaY, float wheel, bool pan)
{
	if (!free_look) return;
	if (pan) {
		// Drag the scene along the screen's right/up axes without rotating.
		const float panSpeed = 0.01f;
		position += (-basis_x * deltaX + basis_y * deltaY) * panSpeed;
	} else if (deltaX != 0.f || deltaY != 0.f) {
		yaw += deltaX * sensitvity;
		pitch += deltaY * sensitvity;
		calc_basis();
	}
	// Positive wheel motion moves forward along the current viewing direction.
	position -= basis_z * (wheel * 0.5f);
}
void Fyredon::Camera::calc_basis()
{
	// Clamp Pitch
	pitch = std::max(pitch_min, std::min(pitch, pitch_max));
	// Clamp Yaw
	//Yaw = std::max(Yaw_Min, std::min(Yaw, Yaw_Max));

	// Calc Target Direction
	float d_x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	float d_y = sin(glm::radians(pitch));
	float d_z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	// Update Basis Vectors 
	basis_z = glm::normalize(glm::vec3(d_x, d_y, d_z));
	basis_x = glm::normalize(glm::cross(ref_up, basis_z));
	basis_y = glm::normalize(glm::cross(basis_z, basis_x));
}

std::ostringstream Fyredon::Camera::debug()
{
	std::ostringstream out;
	out << "======== DEBUG::Camera::BEGIN ========\n"
		<< "X = " << "[" << basis_x.x << "," << basis_x.y << "," << basis_x.z << "]\n"
		<< "Y = " << "[" << basis_y.x << "," << basis_y.y << "," << basis_y.z << "]\n"
		<< "Z = " << "[" << basis_z.x << "," << basis_z.y << "," << basis_z.z << "]\n"
		<< "Yaw = " << yaw << "  Pitch = " << pitch << "\n"
		<< "Pos = " << "[" << position.x << "," << position.y << "," << position.z << "]\n";
	out << "======== DEBUG::Camera::END ========\n";
	return out; 
}