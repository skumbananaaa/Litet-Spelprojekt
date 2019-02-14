#include <EnginePch.h>
#include <Graphics/ParticleSystem.h>
#include <System/Random.h>

ParticleSystem::ParticleSystem()
	: m_pTexture(nullptr),
	m_pParticleInstances(nullptr),
	m_pParticleIndices(nullptr),
	m_NumParticles(0)
{
	m_MaxParticles = 2000;

	m_pParticleIndices = new uint32[m_MaxParticles];
	m_pParticles = new ParticleInternal[m_MaxParticles];
	m_pParticleInstances = new ParticleInstance[m_MaxParticles];
	for (uint32 i = 0; i < m_MaxParticles; i++)
	{
		m_pParticleIndices[i] = i;

		m_pParticles[i].Position = GetPosition();
		m_pParticles[i].Direction = UP_VECTOR;
		m_pParticles[i].TimeLived = 0.0f;
		m_pParticles[i].Speed = 0.0f;

		m_pParticleInstances[i].Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		m_pParticleInstances[i].Position = glm::vec3(0.0f);
	}
}

ParticleSystem::~ParticleSystem()
{
	DeleteArrSafe(m_pParticles);
	DeleteArrSafe(m_pParticleInstances);
}

void ParticleSystem::Update(const Camera& camera, float deltaTime)
{
	for (uint32 i = 0; i < m_ParticlesPerFrame; i++)
	{
		if (m_NumParticles < m_MaxParticles)
		{
			SpawnParticle();
		}
	}

	for (int32 i = m_NumParticles - 1; i >= 0; i--)
	{
		ParticleInternal& particle = GetParticle(i);

		particle.TimeLived += deltaTime;
		if (particle.TimeLived > m_TimeToLive)
		{
			KillParticle(i);
			i--;

			continue;
		}
		
		float lifePercentage = particle.TimeLived / m_TimeToLive;
		particle.Position += (particle.Direction * particle.Speed * deltaTime);

		glm::vec3 distToCamera = particle.Position - camera.GetPosition();
		float lenSqrd = (distToCamera.x * distToCamera.x) + (distToCamera.y * distToCamera.y) + (distToCamera.z * distToCamera.z);

		int32 index = m_NumParticles - i;
		m_pParticleInstances[index].Color = m_Color;
		m_pParticleInstances[index].Color.a = 1.0f - lifePercentage;
		m_pParticleInstances[index].Position = particle.Position;
	}

	GameObject::Update(camera, deltaTime);
}

void ParticleSystem::SetParticlesPerFrame(uint32 particlePerFrame) noexcept
{
	m_ParticlesPerFrame = particlePerFrame;
}

void ParticleSystem::SetSpeed(float min, float max) noexcept
{
	m_MinSpeed = min;
	m_MaxSpeed = max;
}

void ParticleSystem::SetTimeToLive(float timeToLive) noexcept
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

ParticleSystem::ParticleInternal& ParticleSystem::GetParticle(uint32 index) noexcept
{
	return m_pParticles[m_pParticleIndices[index]];
}

void ParticleSystem::SpawnParticle() noexcept
{
	ParticleInternal& particle = GetParticle(m_NumParticles);
	particle.Speed = Random::GenerateFloat(m_MinSpeed, m_MaxSpeed);
	particle.Position = GetPosition();
	particle.Position.z += Random::GenerateFloat(0.0f, 0.05f);
	particle.TimeLived = 0.0f;
	
	m_NumParticles++;
}

void ParticleSystem::KillParticle(uint32 index)
{
	if (index == m_NumParticles - 1)
	{
		m_NumParticles--;
		return;
	}

	uint32 temp = m_pParticleIndices[index];
	m_pParticleIndices[index] = m_pParticleIndices[m_NumParticles - 1];
	m_pParticleIndices[m_NumParticles - 1] = temp;

	m_NumParticles--;
}