#pragma once
#include <Graphics/Geometry/Particle.h>
#include "ParticleData.h"

#define MAX_PARTICLES 50000

class API ParticleManager
{
public:
	ParticleManager(ParticleManager&& other) = delete;
	ParticleManager(const ParticleManager& other) = delete;
	ParticleManager& operator=(ParticleManager&& other) = delete;
	ParticleManager& operator=(const ParticleManager& other) = delete;

	static void SortParticles();

private:
	static ParticleData s_Paricles[MAX_PARTICLES];
	static ParticleInstance s_Instances[MAX_PARTICLES];
};