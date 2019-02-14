#include <EnginePch.h>
#include <Graphics/ParticleSystem.h>

ParticleSystem::ParticleSystem()
	: m_pTexture(nullptr),
	m_pParticleInstances(nullptr),
	m_NumParticles(0)
{
	m_NumParticles = 2000;

	m_pParticles = new ParticleInternal[m_NumParticles];
	m_pParticleInstances = new ParticleInstance[m_NumParticles];
	for (uint32 i = 0; i < m_NumParticles; i++)
	{
		m_pParticles[i].Position = GetPosition();
		m_pParticles[i].Direction = UP_VECTOR;
		m_pParticles[i].TimeLived = 0.0f;
		m_pParticles[i].Speed = 0.5f;

		m_pParticleInstances[i].Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		m_pParticleInstances[i].Model = glm::mat4(0.0f);
	}
}

ParticleSystem::~ParticleSystem()
{
	DeleteArrSafe(m_pParticles);
	DeleteArrSafe(m_pParticleInstances);
}

void ParticleSystem::Update(float deltaTime)
{
	for (uint32 i = 0; i < m_NumParticles; i++)
	{
		m_pParticles[i].TimeLived += deltaTime;
		if (m_pParticles[i].TimeLived > m_TimeToLive)
		{
			m_pParticles[i].Position = GetPosition();
			m_pParticles[i].TimeLived = 0.0f;
		}
		else
		{
			m_pParticles[i].Position += (m_pParticles[i].Direction * m_pParticles[i].Speed * deltaTime);
		}

		m_pParticleInstances[i].Color = m_Color;
		m_pParticleInstances[i].Model = glm::translate(glm::mat4(1.0f), m_pParticles[i].Position);
	}

	GameObject::Update(deltaTime);
}

void ParticleSystem::SetTimeToLive(float timeToLive)
{
	m_TimeToLive = timeToLive;
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

void ParticleSystem::SpawnParticle() noexcept
{
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
