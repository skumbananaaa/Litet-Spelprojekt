#pragma once
#include <EnginePch.h>

class API MESH
{
public:
	static uint32 QUAD;
	static uint32 SHIP;
	static uint32 CLIFF_3_LOW;
	static uint32 CUBE;
	static uint32 CUBE_OBJ;
	static uint32 CHAIR;
	static uint32 SPHERE;
	static uint32 BED_SINGLE;
	static uint32 BED_BUNK;
	static uint32 INSTRUMENT_1;
	static uint32 INSTRUMENT_2;
	static uint32 LAMP;

public:
	static void RegisterResources();
};