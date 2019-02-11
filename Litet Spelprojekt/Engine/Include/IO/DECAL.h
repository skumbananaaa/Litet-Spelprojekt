#pragma once
#include <EnginePch.h>

class API DECAL
{
	friend class ResourceHandler;

public:
	static uint32 BLOOD;

private:
	static void RegisterResourcesPreLoading();
	static void RegisterResources();
};