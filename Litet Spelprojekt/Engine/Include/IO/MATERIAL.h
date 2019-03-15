#pragma once
#include <EnginePch.h>

class API MATERIAL
{
	friend class ResourceHandler;

public:
	static uint32 BLACK;
	static uint32 WHITE;
	static uint32 ARTILLERY;
	static uint32 DOOR_FRAME;
	static uint32 DOOR_RED;
	static uint32 DOOR_GREEN;
	static uint32 DOOR_BLUE;
	static uint32 DOOR_YELLOW;
	static uint32 FIRE_RELATED;
	static uint32 LADDER;
	static uint32 OCEAN_BLUE;
	static uint32 BOAT;
	static uint32 GROUND;
	static uint32 WATER_OUTDOOR;
	static uint32 WATER_INDOOR;
	static uint32 WALL_STANDARD;
	static uint32 BULKHEADS_STANDARD;
	static uint32 CREW_STANDARD;
	static uint32 CREW_INJURED;
	static uint32 SINGLE_BED;
	static uint32 BUNK_BED;
	static uint32 INSTRUMENT_1;
	static uint32 INSTRUMENT_2;
	static uint32 LAMP;
	static uint32 CHAIR;
	static uint32 CUPBOARD;
	static uint32 TABLE;
	static uint32 TOILET;
	static uint32 ANIMATED_MODEL;
	static uint32 ANIMATED_MODEL_EXTINGUISH;
	static uint32 FLOOR_NORMAL;
	static uint32 FLOOR_SICKBAY1;
	static uint32 FLOOR_TOILET1;
	static uint32 FLOOR_MACHINE1;
	static uint32 FLOOR_MACHINE2;
	static uint32 FLOOR_MACHINE3;
	static uint32 FLOOR_AMMUNITION1;
	static uint32 FLOOR_AMMUNITION2;
	static uint32 FLOOR_AMMUNITION3;
	static uint32 FLOOR_KITCHEN1;
	static uint32 FLOOR_DINING1;
	static uint32 FLOOR_CABOOSE1;
	static uint32 FLOOR_EXT_NORMAL;
	static uint32 FLOOR_EXT_SICKBAY1;
	static uint32 FLOOR_EXT_TOILET1;
	static uint32 FLOOR_EXT_MACHINE1;
	static uint32 FLOOR_EXT_MACHINE2;
	static uint32 FLOOR_EXT_MACHINE3;
	static uint32 FLOOR_EXT_AMMUNITION1;
	static uint32 FLOOR_EXT_AMMUNITION2;
	static uint32 FLOOR_EXT_AMMUNITION3;
	static uint32 FLOOR_EXT_KITCHEN1;
	static uint32 FLOOR_EXT_DINING1;
	static uint32 FLOOR_EXT_CABOOSE1;
	static uint32 OVEN;
	static uint32 SHELF_EMPTY;
	static uint32 SHELF_AMMUNITION;
	static uint32 GENERATOR;
	static uint32 FIRE_EXTINGUISHER;
	static uint32 FIRESPRINKLER;
	static uint32 LOOK_AT;
	//REMEMBER, WHEN ADDING NEW MATERIALS, ALSO ADD IT TO THE SceneGame UpdateMaterialClipPlanes() FUNCTION

private:
	static void RegisterResourcesPreLoading();
	static void RegisterResources();
};