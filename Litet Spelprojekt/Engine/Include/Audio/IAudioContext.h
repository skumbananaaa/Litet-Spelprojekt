#pragma once
#include <EnginePch.h>

class API IAudioContext
{
public:
	IAudioContext() {}
	virtual ~IAudioContext() {}

public:
	static IAudioContext* CreateContext();
};