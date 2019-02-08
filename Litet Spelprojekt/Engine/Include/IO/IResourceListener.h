#pragma once
#include <EnginePch.h>

class API IResourceListener
{
public:
	virtual ~IResourceListener() {};
	virtual void OnResourceLoaded(std::string file, float percentage) = 0;
	virtual void OnResourceLoadingFinished() = 0;
};