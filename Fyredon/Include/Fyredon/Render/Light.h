// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	Light.h: Basic Light class (Point lights for now) : Niall
=============================================================================*/
#pragma once

// Ext Headers
#include "glm/glm.hpp"

namespace Fyredon {

class alignas(16) Light
{
public: 
	Light(const glm::vec3& Pos, float Strength);

	glm::vec3 position;
	float strength; 
};

}