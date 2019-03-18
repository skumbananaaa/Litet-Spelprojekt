#include <EnginePch.h>
#include <System\Random.h>
#include <ctime>
#include <cstdlib>

Random Random::s_Instance = Random();

Random::Random()
{
	srand((uint32)time(0));
}

Random::~Random()
{
}

float Random::GenerateFloat(float min, float max)
{
	return ((static_cast<float>(rand()) / (static_cast<float>(RAND_MAX))) * (max - min)) + min;
}

int32 Random::GenerateInt(int32 min, int32 max)
{
	return rand() % (max - min + 1) + min;
}

bool Random::GenerateBool()
{
	return GenerateInt(0, 1);
}
