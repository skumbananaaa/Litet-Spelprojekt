#pragma once
#include <EnginePch.h>

#define MAX_PARTICLES 1000

template<typename T>
struct Node
{
	T Data;
	float AtLifeTime;
};

struct ParticleInstance
{
	glm::vec3 Position;
	glm::vec4 Color;
	glm::vec2 Scale;
};

struct ParticleData
{
public:
	glm::vec4 Color;
	glm::vec3 Position;
	glm::vec3 Direction;
	glm::vec2 Scale;
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