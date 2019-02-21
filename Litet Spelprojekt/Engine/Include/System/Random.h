#pragma once
#include <EnginePch.h>

class API Random
{
public:
	static float GenerateFloat(float min, float max);
	static int32 GenerateInt(int32 min, int32 max);
	static bool GenerateBool();

private:
	Random();
	~Random();

private:
	static Random s_Instance;
};
