#pragma once
#include <EnginePch.h>

class API SOUND
{
	friend class ResourceHandler;

public:
	static uint32 MONO_FART;
	static uint32 MONO_FIREALARM;
	static uint32 UI_SELECT;
	static uint32 UI_DESELECT;
	static uint32 UI_HOVER;
	static uint32 MONO_EXPLOSION;

private:
	static void RegisterResourcesPreLoading();
	static void RegisterResources();
};