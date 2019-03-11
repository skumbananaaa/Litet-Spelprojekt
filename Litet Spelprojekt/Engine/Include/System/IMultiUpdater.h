#pragma once
#include <EnginePch.h>

class API IMultiUpdater
{
public:
	virtual ~IMultiUpdater() {};
	virtual void UpdateParallel(float dtS) noexcept = 0;

protected:
	IMultiUpdater() {};
};