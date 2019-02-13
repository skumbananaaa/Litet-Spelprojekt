#pragma once
#include <EnginePch.h>

class API Random
{
public:
	static int32 GenerateInt(int32 min, int32 max);
	static bool GenerateBool();
private:
	Random();
	static bool init;
};
