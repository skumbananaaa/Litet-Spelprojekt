#pragma once
#include <EnginePch.h>

class API IResourceListener
{
public:
	virtual ~IResourceListener() {};
	virtual void OnLoading(const std::string&, float percentage) = 0;
	virtual void OnResourceLoadingFinished() = 0;
};