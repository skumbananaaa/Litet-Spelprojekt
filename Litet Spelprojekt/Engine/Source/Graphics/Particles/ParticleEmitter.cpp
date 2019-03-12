#include <EnginePch.h>
#include <Graphics/Particles/ParticleEmitter.h>
#include <System/Random.h>
#include <GLMHelper.inl>

ParticleEmitter::ParticleEmitter()
	: m_pTexture(nullptr),
	m_BlendMode(PARTICLE_NORMAL),
	m_BeginScale(1.0f),
	m_EndScale(1.0f),
	m_TimeToLive(1.0f),
	m_MinSpeed(1.0f),
	m_MaxSpeed(2.0f),
	m_ParticleBacklog(0.0f),
	m_ConeAngle(0.0f),
	m_NumSortedParticles(0),
	m_NumParticles(0),
	m_ParticlesPerSecond(1),
	m_Direction(UP_VECTOR),
	m_ColorNodes(),
	m_LivingParticles(),
	m_SortedParticles(),
	m_Particles(),
	m_ParticleInstances()
{
	//Set particleemitters to be updated
	m_IsTickable = true;

	for (uint32 i = 0; i < MAX_PARTICLES; i++)
	{
		m_LivingParticles[i] = i;
		m_SortedParticles[i] = i;

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

ParticleEmitter::~ParticleEmitter()
{
}

void ParticleEmitter::Update(const Camera& camera, float deltaTime) noexcept
{
	GameObject::Update(camera, deltaTime);
	
	//Get distance to camera
	m_DistToCamera = LengthSqrd(GetPosition() - camera.GetPosition());

	//Spawn particles
	float particlesThisFrame = m_ParticleBacklog + (m_ParticlesPerSecond * deltaTime);
	for (; particlesThisFrame >= 1.0f; particlesThisFrame -= 1.0f)
	{
		SpawnParticle();
	}
	m_ParticleBacklog = particlesThisFrame;

	//Update and sort particles
	m_NumSortedParticles = 0;
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

		InsertSortedParticle(m_LivingParticles[i]);
	}

	//Fill instances
	for (uint32 i = 0; i < m_NumSortedParticles; i++)
	{
		ParticleData& data = GetSortedParticle(i);
		m_ParticleInstances[i].Color = data.Color;
		m_ParticleInstances[i].Color.a = data.Color.a * (1.0f - data.LifePercentage);
		m_ParticleInstances[i].Position = data.Position;
		m_ParticleInstances[i].Scale = data.Scale;
	}
}

void ParticleEmitter::UpdateTransform() noexcept
{
	GameObject::UpdateTransform();
	
	if (m_IsDirty)
	{
		m_Direction = GetTransform() * glm::vec4(UP_VECTOR, 0.0f);
		m_Direction = glm::normalize(m_Direction);
	}
}

void ParticleEmitter::SetParticlesPerSeconds(uint32 numParticles) noexcept
{
	m_ParticlesPerSecond = numParticles;
}

void ParticleEmitter::SetParticleBlendMode(ParticleBlendMode blendMode) noexcept
{
	m_BlendMode = blendMode;
}

ParticleBlendMode ParticleEmitter::GetParticleBlendMode() const noexcept
{
	return m_BlendMode;
}

bool ParticleEmitter::DistLess(ParticleEmitter* pEmitter1, ParticleEmitter* pEmitter2)
{
	return pEmitter1->m_DistToCamera < pEmitter2->m_DistToCamera;
}

bool ParticleEmitter::DistGreater(ParticleEmitter* pEmitter1, ParticleEmitter* pEmitter2)
{
	return pEmitter1->m_DistToCamera > pEmitter2->m_DistToCamera;
}

void ParticleEmitter::SetConeAngle(float angleRad) noexcept
{
	m_ConeAngle = abs(angleRad);
}

void ParticleEmitter::SetScale(const glm::vec2& begin, const glm::vec2& end) noexcept
{
	m_BeginScale = begin;
	m_EndScale = end;
}

void ParticleEmitter::SetSpeed(float min, float max) noexcept
{
	m_MinSpeed = min;
	m_MaxSpeed = max;
}

void ParticleEmitter::SetTimeToLive(float timeToLive) noexcept
{
	m_TimeToLive = timeToLive;
}

void ParticleEmitter::SetTexture(uint32 textureID) noexcept
{
	m_pTexture = ResourceHandler::GetTexture2D(textureID);
}

void ParticleEmitter::AddColorNode(const glm::vec4& color, float atLifeTime) noexcept
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

void ParticleEmitter::SetBeginColor(const glm::vec4& color) noexcept
{
	m_ColorNodes[0].Data = color;
}

Node<glm::vec4>& ParticleEmitter::GetColorNode(uint32 index) noexcept
{
	return m_ColorNodes[index];
}

const Node<glm::vec4>& ParticleEmitter::GetColorNode(uint32 index) const noexcept
{
	return m_ColorNodes[index];
}

void ParticleEmitter::SetEndColor(const glm::vec4& color) noexcept
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

uint32 ParticleEmitter::GetNumParticles() const noexcept
{
	return m_NumParticles;
}

float ParticleEmitter::GetDistToCamera() const noexcept
{
	return m_DistToCamera;
}

const Texture2D* ParticleEmitter::GetTexture() const noexcept
{
	return m_pTexture;
}

const ParticleInstance* ParticleEmitter::GetParticleInstances() const noexcept
{
	return m_ParticleInstances;
}

ParticleData& ParticleEmitter::GetLivingParticle(uint32 index) noexcept
{
	return m_Particles[m_LivingParticles[index]];
}

ParticleData& ParticleEmitter::GetSortedParticle(uint32 index) noexcept
{
	return m_Particles[m_SortedParticles[index]];
}

void ParticleEmitter::InsertSortedParticle(uint32 id) noexcept
{
	ParticleData& particle = m_Particles[id];

	int32 index = m_NumSortedParticles;
	for (int32 i = m_NumSortedParticles - 1; i > 0; i--)
	{
		if (particle > GetSortedParticle(i))
		{
			index = i;
			memcpy(&m_SortedParticles[i + 1], &m_SortedParticles[i], sizeof(uint32) * (m_NumSortedParticles - i));
			break;
		}
	}

	m_SortedParticles[index] = id;
	m_NumSortedParticles++;
}

void ParticleEmitter::SpawnParticle() noexcept
{
	if (m_NumParticles < MAX_PARTICLES)
	{
		ParticleData& particle = GetLivingParticle(m_NumParticles);
		particle.Speed = Random::GenerateFloat(m_MinSpeed, m_MaxSpeed);
		particle.Position = GetPosition();


		float dDotD = glm::dot(m_Direction, m_Direction);
		dDotD = (dDotD < 0.00001f) ? 0.00001f : dDotD;

		glm::vec3 forward = glm::normalize(FORWARD_VECTOR - ((glm::dot(m_Direction, FORWARD_VECTOR) / dDotD) * FORWARD_VECTOR));
		glm::vec4 dir = glm::rotate(glm::mat4(1.0f), Random::GenerateFloat(0.0f, m_ConeAngle), forward) * glm::vec4(m_Direction, 0.0f);
		dir = glm::rotate(glm::mat4(1.0f), Random::GenerateFloat(0.0f, glm::two_pi<float>()), m_Direction) * dir;
		
		particle.Direction.x = dir.x;
		particle.Direction.y = dir.y;
		particle.Direction.z = dir.z;

		particle.TimeLived = 0.0f;
		particle.DistToCameraSqrd = 0.0f;

		m_NumParticles++;
	}
}

void ParticleEmitter::KillParticle(uint32 index)
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