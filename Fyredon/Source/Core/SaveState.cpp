#include "Fyredon/Other/pch.h"
#include "Fyredon/Core/SaveState.h"



/*
	TODO
	=====
	- User chosen directory/filename for dat file
	- Lights + properties stored
	- Assets stored
*/

Fyredon::SaveState Fyredon::SaveState::instance;

Fyredon::SaveState::SaveState() : cam_pos(glm::vec3(0.0f)), update(0), yaw(0.0f), pitch(0.0f) {

}

// Get the singleton instance of the SaveState class (intended to be thread-safe)
Fyredon::SaveState& Fyredon::SaveState::getInstance()
{
	return instance;
}


// Updates all the state variables to be packaged into the binary, and encode it
void Fyredon::SaveState::tick(glm::vec3& cam_pos, float yaw, float pitch)
{
	this->cam_pos = cam_pos;
	this->yaw = yaw;
	this->pitch = pitch;

	encode();

	update = false;
	//FD_DEBUG_INFO("fffff", cam_pos.x, cam_pos.y, cam_pos.z, yaw, pitch);
}


void Fyredon::SaveState::load()
{
	decode();
	loaded = false; // finished loading state
}


// Encode state into binary file
/*
	In order from top to bottom:

	cam_pos.x, cam_pos.y, cam_pos.z		- 12 bytes
	yaw									- 4 bytes
	pitch								- 4 bytes
*/
void Fyredon::SaveState::encode()
{
	std::ofstream out;
	out.open("../Out/state.dat", std::ios::out | std::ios::binary);

	// Write camera stuff in
	out.write(reinterpret_cast<const char*>(&cam_pos.x), sizeof(float));
	out.write(reinterpret_cast<const char*>(&cam_pos.y), sizeof(float));
	out.write(reinterpret_cast<const char*>(&cam_pos.z), sizeof(float));
	out.write(reinterpret_cast<const char*>(&yaw), sizeof(float));
	out.write(reinterpret_cast<const char*>(&pitch), sizeof(float));

	out.close();
}


// Decode state into memory
void Fyredon::SaveState::decode()
{
	std::ifstream in("../Out/state.dat", std::ios::binary);

	// Read camera stuff in
	in.read(reinterpret_cast<char*>(&cam_pos.x), sizeof(float));
	in.read(reinterpret_cast<char*>(&cam_pos.y), sizeof(float));
	in.read(reinterpret_cast<char*>(&cam_pos.z), sizeof(float));
	in.read(reinterpret_cast<char*>(&yaw), sizeof(float));
	in.read(reinterpret_cast<char*>(&pitch), sizeof(float));

	
}