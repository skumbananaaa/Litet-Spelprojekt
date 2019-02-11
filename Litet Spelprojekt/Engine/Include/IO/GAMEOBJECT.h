#pragma once
#include <EnginePch.h>

class API GAMEOBJECT
{
	friend class ResourceHandler;

public:
	static uint32 CHAIR;
	static uint32 BED_SINGLE;
	static uint32 BED_BUNK;
	static uint32 INSTRUMENT_1;
	static uint32 INSTRUMENT_2;

private:
	static void RegisterResourcesPreLoading();
	static void RegisterResources();
};