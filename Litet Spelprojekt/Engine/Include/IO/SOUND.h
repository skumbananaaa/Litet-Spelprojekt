#pragma once
#include <EnginePch.h>

class API SOUND
{
	friend class ResourceHandler;

public:
	static uint32 MONO_FART;

private:
	static void RegisterResourcesPreLoading();
	static void RegisterResources();
};