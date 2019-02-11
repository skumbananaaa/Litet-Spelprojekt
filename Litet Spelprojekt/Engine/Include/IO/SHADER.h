#pragma once
#include <EnginePch.h>

class API SHADER
{
	friend class ResourceHandler;

public:
	static uint32 GUI;
	static uint32 FONT;

private:
	static void RegisterResourcesPreLoading();
	static void RegisterResources();
};