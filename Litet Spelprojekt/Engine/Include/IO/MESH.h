#pragma once
#include <EnginePch.h>

class API MESH
{
	friend class ResourceHandler;

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
	static uint32 ANIMATED_MODEL_RUN;
	static uint32 ANIMATED_MODEL_IDLE;
	static uint32 ANIMATED_MODEL_WORK;
	static uint32 ANIMATED_MODEL_EAT;
	static uint32 ANIMATED_MODEL_OPENDOOR;
	static uint32 ANIMATED_MODEL_SLEEP;
	static uint32 ANIMATED_MODEL_SITTING;
	static uint32 DOOR;
	static uint32 DOOR_FRAME;
	static uint32 LADDER;
	static uint32 MESH_PARTICLE;
	static uint32 CUPBOARD;
	static uint32 TABLE;
	static uint32 TOILET;
	static uint32 WATER_QUAD;
	static uint32 ARTILLERY;
	static uint32 OVEN;
	static uint32 SHELF_AMMUNITION;
	static uint32 SHELF_EMPTY;
	static uint32 GENERATOR;
	static uint32 FIRE_EXTINGUISHER;

private:
	static void RegisterResourcesPreLoading();
	static void RegisterResources();
};