#pragma once
#include <EnginePch.h>

class API MATERIAL
{
	friend class ResourceHandler;

public:
	static uint32 BLACK;
	static uint32 WHITE;
	static uint32 RED;
	static uint32 GREEN;
	static uint32 BLUE;
	static uint32 YELLOW;
	static uint32 OCEAN_BLUE;
	static uint32 BOAT;
	static uint32 GROUND;
	static uint32 WATER_OUTDOOR;
	static uint32 WATER_INDOOR;
	static uint32 WALL_STANDARD;
	static uint32 BULKHEADS_STANDARD;
	static uint32 CREW_STANDARD;
	static uint32 SINGLE_BED;
	static uint32 BUNK_BED;
	static uint32 INSTRUMENT_1;
	static uint32 INSTRUMENT_2;
	static uint32 LAMP;
	static uint32 CHAIR;
	static uint32 CUBOARD;
	static uint32 TABLE;
	static uint32 TOILET;
	static uint32 ANIMATED_MODEL;
	static uint32 FLOOR_NORMAL;
	static uint32 FLOOR_SICKBAY;
	static uint32 FLOOR_MACHINE1;
	static uint32 FLOOR_MACHINE2;
	static uint32 FLOOR_MACHINE3;
	static uint32 FLOOR_AMMUNITION1;
	static uint32 FLOOR_AMMUNITION2;
	static uint32 FLOOR_AMMUNITION3;

private:
	static void RegisterResourcesPreLoading();
	static void RegisterResources();
};