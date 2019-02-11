#pragma once
#include <EnginePch.h>

class API MATERIAL
{
public:
	static uint32 BLACK;
	static uint32 WHITE;
	static uint32 RED;
	static uint32 RED_1;
	static uint32 RED_2;
	static uint32 RED_3;
	static uint32 RED_4;
	static uint32 GREEN;
	static uint32 BLUE;
	static uint32 BOAT;
	static uint32 GROUND;
	static uint32 WATER;
	static uint32 WALL_STANDARD;
	static uint32 CREW_STANDARD;
	static uint32 SINGLE_BED;
	static uint32 BUNK_BED;

public:
	static void RegisterResources();
};