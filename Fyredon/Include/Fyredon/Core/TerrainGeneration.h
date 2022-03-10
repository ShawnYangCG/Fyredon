// Copyright Fyredon. All Rights Reserved.

/*=============================================================================
	TerrainGeneration.h: Terrain Generation
=============================================================================*/


#ifndef	TERRAIN_GENERATION_H
#define TERRAIN_GENERATION_H

#include <filesystem>
#include <iostream>
#include <vector>
//#include "Fyredon/Extern/stb/stb_image.h"


using namespace std;

class TerrainGenerator
{
public:
	TerrainGenerator();
	~TerrainGenerator();

	void read_height_map(string path);



private:
};

#endif