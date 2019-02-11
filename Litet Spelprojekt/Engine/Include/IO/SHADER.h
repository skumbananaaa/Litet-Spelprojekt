#pragma once
#include <EnginePch.h>

class API SHADER
{
	friend class ResourceHandler;

public:
	static uint32 GUI;
	static uint32 FONT;
	static uint32 DEBUG_DECALS;
	static uint32 DEFERRED_WALL;

private:
	static void RegisterResourcesPreLoading();
	static void RegisterResources();
};