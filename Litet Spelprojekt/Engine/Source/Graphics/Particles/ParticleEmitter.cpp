#include <EnginePch.h>
#include <Graphics/Particles/ParticleEmitter.h>
#include <Graphics/Particles/ParticleSystem.h>

ParticleEmitter::ParticleEmitter(ParticleSystem* pSystem)
	: m_pSystem(pSystem),
	m_ParticlesPerFrame(1),
	m_Direction(0.0f)
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
		m_pSystem->SpawnParticle(GetPosition(), m_Direction);
	}

	GameObject::Update(deltaTime);
}

void ParticleEmitter::UpdateTransform() noexcept
{
	GameObject::UpdateTransform();
	m_Direction = GetTransform() * glm::vec4(UP_VECTOR, 0.0f);
	m_Direction = glm::normalize(m_Direction);
}

void ParticleEmitter::SetParticlesPerFrame(int32 numParticles) noexcept
{
	m_ParticlesPerFrame = numParticles;
}