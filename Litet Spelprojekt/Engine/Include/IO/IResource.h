#pragma once
#include <EnginePch.h>

class API IResource
{
public:
	virtual ~IResource() {};

private:
	virtual void Construct() = 0;
};