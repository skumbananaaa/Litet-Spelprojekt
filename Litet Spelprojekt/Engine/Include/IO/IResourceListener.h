#pragma once
#include <EnginePch.h>

class API IResourceListener
{
public:
	virtual void OnResourcesLoaded() = 0;
};