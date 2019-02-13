#include <EnginePch.h>
#include <Graphics/ParticleSystem.h>

ParticleSystem::ParticleSystem()
	: m_pTexture(nullptr),
	m_pParticleInstances(nullptr),
	m_NumParticles(0)
{
	m_NumParticles = 10;
	m_pParticleInstances = new ParticleInstance[m_NumParticles];
	for (uint32 i = 0; i < m_NumParticles; i++)
	{
		m_pParticleInstances[i].Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		m_pParticleInstances[i].Model = GetTransform();
	}
}

ParticleSystem::~ParticleSystem()
{
	DeleteArrSafe(m_pParticleInstances);
}

void ParticleSystem::Update(float deltaTime)
{
	for (uint32 i = 0; i < m_NumParticles; i++)
	{
		m_pParticleInstances[i].Color = m_Color;
		m_pParticleInstances[i].Model = GetTransform();
	}

	GameObject::Update(deltaTime);
}

void ParticleSystem::SetTexture(uint32 textureID) noexcept
{
	m_pTexture = ResourceHandler::GetTexture2D(textureID);
}

void ParticleSystem::SetColor(const glm::vec4& color) noexcept
{
	m_Color = color;
}

uint32 ParticleSystem::GetNumParticles() const noexcept
{
	return m_NumParticles;
}

const Texture2D* ParticleSystem::GetTexture() const noexcept
{
	return m_pTexture;
}

const ParticleInstance* ParticleSystem::GetParticleInstances() const noexcept
{
	return m_pParticleInstances;
}

const glm::vec4& ParticleSystem::GetColor() const noexcept
{
	return m_Color;
}
