#include <EnginePch.h>
#include <Graphics/Particles/MeshEmitter.h>
#include <System/Random.h>
#include <GLMHelper.inl>

MeshEmitter::MeshEmitter()
	: m_BeginScale(1.0f),
	m_EndScale(1.0f),
	m_TimeToLive(1.0f),
	m_MinSpeed(1.0f),
	m_MaxSpeed(2.0f),
	m_ParticleBacklog(0.0f),
	m_ConeAngle(0.0f),
	m_NumParticles(0),
	m_ParticlesPerSecond(1),
	m_Direction(0.0f),
	m_ColorNodes(),
	m_LivingParticles(),
	m_Particles(),
	m_ParticleInstances()
{
	for (uint32 i = 0; i < MAX_PARTICLES; i++)
	{
		m_Particles[i].Position = glm::vec3(0.0f);
		m_Particles[i].Direction = UP_VECTOR;
		m_Particles[i].TimeLived = 0.0f;
		m_Particles[i].Speed = 0.0f;
		m_Particles[i].DistToCameraSqrd = 0.0f;

		m_ParticleInstances[i].Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		m_ParticleInstances[i].Position = glm::vec3(0.0f);
		m_ParticleInstances[i].Scale = glm::vec2(1.0f);
	}

	Node<glm::vec4> beginColor = { glm::vec4(1.0f), 0.0f };
	m_ColorNodes.push_back(beginColor);
}

MeshEmitter::~MeshEmitter()
{
}

void MeshEmitter::Update(const Camera& camera, float deltaTime) noexcept
{
	GameObject::Update(camera, deltaTime);

	//Spawn particles
	float particlesThisFrame = m_ParticleBacklog + (m_ParticlesPerSecond * deltaTime);
	for (; particlesThisFrame >= 1.0f; particlesThisFrame -= 1.0f)
	{
		SpawnParticle();
	}
	m_ParticleBacklog = particlesThisFrame;

	//Update and sort particles
	for (uint32 i = 0; i < GetNumParticles(); i++)
	{
		//Update
		ParticleData& particle = GetLivingParticle(i);
		particle.TimeLived += deltaTime;
		if (particle.TimeLived > m_TimeToLive)
		{
			KillParticle(i);
			i--;

			continue;
		}

		//Position
		particle.Position += (particle.Direction * particle.Speed * deltaTime);
		particle.LifePercentage = particle.TimeLived / m_TimeToLive;
		particle.DistToCameraSqrd = LengthSqrd(particle.Position - camera.GetPosition());

		//Color
		int32 colorCurrentIndex = 0;
		for (size_t i = 0; i < m_ColorNodes.size(); i++)
		{
			if (particle.LifePercentage >= m_ColorNodes[i].AtLifeTime)
			{
				colorCurrentIndex = i;
			}
		}

		int32 colorNextIndex = colorCurrentIndex;
		if ((m_ColorNodes.size() - 1 > colorCurrentIndex))
		{
			colorNextIndex++;
		}

		glm::vec4 diffColor = m_ColorNodes[colorNextIndex].Data - m_ColorNodes[colorCurrentIndex].Data;
		particle.Color = m_ColorNodes[colorCurrentIndex].Data + (diffColor * particle.LifePercentage);

		//Scale
		glm::vec2 diffScale = m_EndScale - m_BeginScale;
		particle.Scale = m_BeginScale + (diffScale * particle.LifePercentage);
	}

	//Fill instances
	for (uint32 i = 0; i < m_NumParticles; i++)
	{
		ParticleData& data = GetLivingParticle(i);
		m_ParticleInstances[i].Color = data.Color;
		m_ParticleInstances[i].Color.a = data.Color.a * (1.0f - data.LifePercentage);
		m_ParticleInstances[i].Position = data.Position;
		m_ParticleInstances[i].Scale = data.Scale;
	}
}

void MeshEmitter::UpdateTransform() noexcept
{
	bool isDirty = IsDirty();

	GameObject::UpdateTransform();

	if (isDirty)
	{
		m_Direction = GetTransform() * glm::vec4(UP_VECTOR, 0.0f);
		m_Direction = glm::normalize(m_Direction);
	}
}

void MeshEmitter::SetMesh(uint32 meshID) noexcept
{
	m_pMesh = ResourceHandler::GetMeshParticle(meshID);
}

void MeshEmitter::SetParticlesPerSeconds(uint32 numParticles) noexcept
{
	m_ParticlesPerSecond = numParticles;
}

void MeshEmitter::SetConeAngle(float angleRad) noexcept
{
	m_ConeAngle = abs(angleRad);
}

void MeshEmitter::SetScale(const glm::vec2& begin, const glm::vec2& end) noexcept
{
	m_BeginScale = begin;
	m_EndScale = end;
}

void MeshEmitter::SetSpeed(float min, float max) noexcept
{
	m_MinSpeed = min;
	m_MaxSpeed = max;
}

void MeshEmitter::SetTimeToLive(float timeToLive) noexcept
{
	m_TimeToLive = timeToLive;
}

void MeshEmitter::AddColorNode(const glm::vec4& color, float atLifeTime) noexcept
{
	Node<glm::vec4> node;
	node.Data = color;

	if (atLifeTime > 1.0f)
	{
		node.AtLifeTime = 1.0f;
	}
	else if (atLifeTime < 0.0f)
	{
		node.AtLifeTime = 0.0f;
	}
	else
	{
		node.AtLifeTime = atLifeTime;
	}

	m_ColorNodes.push_back(node);
}

void MeshEmitter::SetBeginColor(const glm::vec4& color) noexcept
{
	m_ColorNodes[0].Data = color;
}

Node<glm::vec4>& MeshEmitter::GetColorNode(uint32 index) noexcept
{
	return m_ColorNodes[index];
}

const Node<glm::vec4>& MeshEmitter::GetColorNode(uint32 index) const noexcept
{
	return m_ColorNodes[index];
}

void MeshEmitter::SetEndColor(const glm::vec4& color) noexcept
{
	if (m_ColorNodes.size() > 1)
	{
		if (m_ColorNodes[m_ColorNodes.size() - 1].AtLifeTime >= 1.0f)
		{
			m_ColorNodes[m_ColorNodes.size() - 1].Data = color;
			return;
		}
	}

	Node<glm::vec4> node = { color, 1.0f };
	m_ColorNodes.push_back(node);
}

uint32 MeshEmitter::GetNumParticles() const noexcept
{
	return m_NumParticles;
}

const ParticleInstance* MeshEmitter::GetParticleInstances() const noexcept
{
	return m_ParticleInstances;
}

ParticleData& MeshEmitter::GetLivingParticle(uint32 index) noexcept
{
	return m_Particles[m_LivingParticles[index]];
}

void MeshEmitter::SpawnParticle() noexcept
{
	if (m_NumParticles < MAX_PARTICLES)
	{
		ParticleData& particle = GetLivingParticle(m_NumParticles);
		particle.Speed = Random::GenerateFloat(m_MinSpeed, m_MaxSpeed);
		particle.Position = GetPosition();

		glm::vec3 forward = FORWARD_VECTOR - ((glm::dot(m_Direction, FORWARD_VECTOR) / glm::dot(m_Direction, m_Direction)) * FORWARD_VECTOR);
		glm::vec4 dir = glm::rotate(glm::mat4(1.0f), Random::GenerateFloat(0.0f, m_ConeAngle), glm::normalize(forward)) * glm::vec4(m_Direction, 0.0f);
		dir = glm::rotate(glm::mat4(1.0f), Random::GenerateFloat(0.0f, glm::two_pi<float>()), m_Direction) * dir;
		particle.Direction.x = dir.x;
		particle.Direction.y = dir.y;
		particle.Direction.z = dir.z;

		particle.TimeLived = 0.0f;
		particle.DistToCameraSqrd = 0.0f;

		m_NumParticles++;
	}
}

void MeshEmitter::KillParticle(uint32 index) noexcept
{
	if (index == m_NumParticles - 1)
	{
		m_NumParticles--;
		return;
	}

	uint32 temp = m_LivingParticles[index];
	m_LivingParticles[index] = m_LivingParticles[m_NumParticles - 1];
	m_LivingParticles[m_NumParticles - 1] = temp;

	m_NumParticles--;
}

const MeshParticle* MeshEmitter::GetMesh() const noexcept
{
	return m_pMesh;
}
