// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	Light.h: Basic Light Classes : Niall
=============================================================================*/
#pragma once

// Ext Headers
#include "glm/glm.hpp"

namespace Fyredon {

struct alignas(16) Light_Directional
{
	Light_Directional(const glm::vec3& Direction, const glm::vec3& Colour, float Strength)
		: direction(Direction), colour(Colour), strength(Strength) {}

	glm::vec3 colour, direction;
	float strength;
};

struct alignas(16) Light_Point
{
	Light_Point(const glm::vec3& Position, const glm::vec3& Colour, float Strength)
		: position(Position), colour(Colour), strength(Strength) {}

	glm::vec3 colour, position;
	float strength;
};

}