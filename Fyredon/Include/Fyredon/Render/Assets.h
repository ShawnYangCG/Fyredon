// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	Assets.h: Vertex Data for use for meshes : Niall
=============================================================================*/
#pragma once

#include "Fyredon/Render/Mesh.h"

#include <vector>

namespace Fyredon::Assets
{
	static std::vector<Vert> frustum_verts =
	{
		Vert(0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f, 1.f, 0.f, 0.f, 0.f),
		Vert(.5f, .5f, .5f, 0.f, 0.f, 0.f, 1.f, 1.f, 1.f, 0.f, 0.f, 0.f),
		Vert(0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, -1.f, -1.f, 0.f, 0.f, 0.f),
		Vert(.5f, -.5f, -.5f, 0.f, 0.f, 0.f, 1.f, -1.f, -1.f, 0.f, 0.f, 0.f),
		Vert(0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, -1.f, 1.f, 0.f, 0.f, 0.f),
		Vert(.5f, -.5f, .5f, 0.f, 0.f, 0.f, 1.f, -1.f, 1.f, 0.f, 0.f, 0.f),
		Vert(0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f, -1.f, 0.f, 0.f, 0.f),
		Vert(.5f, .5f, -.5f, 0.f, 0.f, 0.f, 1.f, 1.f, -1.f, 0.f, 0.f, 0.f),
		Vert(.5f, -.5f, .5f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f),
		Vert(.5f, .5f, .5f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f),
		Vert(.5f, .5f, .5f, 0.f, 0.f, 0.f, 0.f, 0.f, -1.f, 0.f, 0.f, 0.f),
		Vert(.5f, .5f, -.5f, 0.f, 0.f, 0.f, 0.f, 0.f, -1.f, 0.f, 0.f, 0.f),
		Vert(.5f, .5f, -.5f, 0.f, 0.f, 0.f, 0.f, -1.f, 0.f, 0.f, 0.f, 0.f),
		Vert(.5f, -.5f, -.5f, 0.f, 0.f, 0.f, 0.f, -1.f, 0.f, 0.f, 0.f, 0.f),
		Vert(.5f, -.5f, -.5f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f),
		Vert(.5f, -.5f, .5f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f)
	};

	static std::vector<Vert> axis_verts =
	{
		Vert(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f)),
		Vert(glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f)),
		Vert(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 0.f, 0.f)),
		Vert(glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 0.f, 0.f)),
		Vert(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 0.f, 0.f)),
		Vert(glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 0.f, 0.f))
	};
}

