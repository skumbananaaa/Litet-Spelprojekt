#pragma once
#include <EnginePch.h>

class API IRunnable
{
public:
	virtual ~IRunnable() {};
	virtual void RunParallel() = 0;

protected:
	IRunnable() {};
};