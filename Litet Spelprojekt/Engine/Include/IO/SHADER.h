#pragma once
#include <EnginePch.h>

class API SHADER
{
	friend class ResourceHandler;

public:
	static uint32 GUI;
	static uint32 FONT;
	static uint32 DEBUG_DECALS;
	static uint32 CBR_RESOLVE;
	static uint32 CBR_RECONSTRUCTION;
	static uint32 CBR_BLUR;
	static uint32 DEPTH_PRE_PASS;
	static uint32 FORWARD_PASS;
	static uint32 SKYBOX_PASS;
	static uint32 ORTHOGRAPHIC;
	static uint32 WALL_MATERIAL;
	static uint32 WATER_OUTDOOR_MATERIAL;
	static uint32 WATER_INDOOR_MATERIAL;
	static uint32 DEFERRED_DECALS;
	static uint32 STANDARD_MATERIAL;
	static uint32 PARTICLES;
	static uint32 EQUIREC_TO_CUBEMAP;

private:
	static void RegisterResourcesPreLoading();
	static void RegisterResources();
};