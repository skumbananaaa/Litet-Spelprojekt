#pragma once
#include <EnginePch.h>
#include <World/World.h>

class API WorldSerializer
{
public:
	static void Read(const char* const path, World& world);
	static void Write(const char* const path, const World& world);
};