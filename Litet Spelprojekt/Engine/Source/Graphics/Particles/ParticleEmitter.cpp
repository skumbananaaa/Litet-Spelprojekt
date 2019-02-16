#include <EnginePch.h>
#include <Graphics/Particles/ParticleEmitter.h>
#include <Graphics/Particles/ParticleSystem.h>

ParticleEmitter::ParticleEmitter(ParticleSystem* pSystem)
	: m_pSystem(pSystem)
{
}

ParticleEmitter::~ParticleEmitter()
{
}

void ParticleEmitter::Update(float deltaTime) noexcept
{
	//Spawn particles
	for (uint32 i = 0; i < m_ParticlesPerFrame; i++)
	{
		m_pSystem->SpawnParticle(GetPosition());
	}

	GameObject::Update(deltaTime);
}

void ParticleEmitter::SetParticlesPerFrame(int32 numParticles) noexcept
{
	m_ParticlesPerFrame = numParticles;
}
