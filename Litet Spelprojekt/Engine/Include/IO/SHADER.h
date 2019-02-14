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
	static uint32 DEFERRED_WALL;
	static uint32 DEFERRED_WATER;
	static uint32 DEFERRED_DECALS;
	static uint32 DEFERRED_MATERIAL;
	static uint32 DEFERRED_PARTICLES;
	static uint32 EQUIREC_TO_CUBEMAP;

private:
	static void RegisterResourcesPreLoading();
	static void RegisterResources();
};