#pragma once
#include <EnginePch.h>

class API MUSIC
{
	friend class ResourceHandler;

public:
	static uint32 WAVES_AND_SEAGULLS;

private:
	static void RegisterResourcesPreLoading();
	static void RegisterResources();
};