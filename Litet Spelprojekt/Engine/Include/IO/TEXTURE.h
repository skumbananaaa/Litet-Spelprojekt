#pragma once
#include <EnginePch.h>

class API TEXTURE
{
	friend class ResourceHandler;

public:
	static uint32 BLOOD;
	static uint32 BLOOD_NORMAL;
	static uint32 SHIP;
	static uint32 SHIP_NORMAL;
	static uint32 WATER_DISTORTION;
	static uint32 WATER_NORMAL;
	static uint32 HDR;
	static uint32 SINGLE_BED;
	static uint32 BUNK_BED;

private:
	static void RegisterResourcesPreLoading();
	static void RegisterResources();
};