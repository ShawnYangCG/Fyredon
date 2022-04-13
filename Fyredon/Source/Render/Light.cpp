// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	Light.cpp: Basic Light class (Point lights for now) : Niall
=============================================================================*/
#include "Fyredon/Other/pch.h"
// Implements
#include "Fyredon/Render/Light.h"

Fyredon::Light::Light(const glm::vec3& Pos, float Strength)
	: position(Pos), strength(Strength) {}

// ToDo : ...
// Base Light class, Point, Directional Lights. 