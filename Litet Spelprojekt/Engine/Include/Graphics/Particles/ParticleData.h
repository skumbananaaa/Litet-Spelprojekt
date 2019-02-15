#pragma once
#include <EnginePch.h>

struct ParticleData
{
public:
	glm::vec3 Position;
	glm::vec3 Direction;
	float Speed;
	float TimeLived;
	float DistToCameraSqrd;
	float LifePercentage;

public:
	inline bool operator<(const ParticleData& other)
	{
		return (this->DistToCameraSqrd < other.DistToCameraSqrd);
	}

	inline bool operator>(const ParticleData& other)
	{
		return (this->DistToCameraSqrd > other.DistToCameraSqrd);
	}
};