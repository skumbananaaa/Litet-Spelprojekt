#pragma once
#include <EnginePch.h>
#include <World/World.h>

class API WorldSerializer
{
public:
	static World* Read(const char* const path);
	static void Write(const char* const path, const World& world);
};