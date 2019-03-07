#pragma once
#include <EnginePch.h>

class GameObject;

class API GAMEOBJECT
{
	friend class ResourceHandler;

public:
	static uint32 CHAIR;
	static uint32 BED_SINGLE;
	static uint32 BED_BUNK;
	static uint32 INSTRUMENT_1;
	static uint32 INSTRUMENT_2;
	static uint32 LAMP;
	static uint32 FIREALARM;
	static uint32 CUBOARD;
	static uint32 TABLE;
	static uint32 TOILET;
	static uint32 DOOR;
	static uint32 FLOOR;

private:
	static void RegisterResourcesPreLoading();
	static void RegisterResources();
};