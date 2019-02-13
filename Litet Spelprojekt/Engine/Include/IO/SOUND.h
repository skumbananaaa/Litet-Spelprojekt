#pragma once
#include <EnginePch.h>

class API SOUND
{
	friend class ResourceHandler;

public:
	static uint32 MONO_FART;
	static uint32 MONO_FIREALARM;

private:
	static void RegisterResourcesPreLoading();
	static void RegisterResources();
};