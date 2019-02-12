#pragma once
#include "GameObject.h"

class API ParticleSystem : public GameObject
{
public:
	ParticleSystem(ParticleSystem&& other) = delete;
	ParticleSystem(const ParticleSystem& other) = delete;
	ParticleSystem& operator=(ParticleSystem&& other) = delete;
	ParticleSystem& operator=(const ParticleSystem& other) = delete;

	ParticleSystem() {}
	~ParticleSystem() {}
};