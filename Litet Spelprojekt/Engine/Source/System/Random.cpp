#include <EnginePch.h>
#include <System\Random.h>
#include <time.h>
#include <stdlib.h>

bool Random::init = false;

Random::Random()
{

}

int32 Random::GenerateInt(int32 min, int32 max)
{
	if (!init)
	{
		srand(time(NULL));
		init = true;
	}
	return rand() % (max - min + 1) + min;
}

bool Random::GenerateBool()
{
	return GenerateInt(0, 1);
}
