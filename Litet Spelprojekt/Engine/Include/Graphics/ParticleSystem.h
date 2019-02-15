#pragma once
#include "GameObject.h"
#include "Geometry/Particle.h"

class API ParticleSystem : public GameObject
{
	friend class DefferedRenderer;

private:
	struct ParticleData
	{
	public:
		glm::vec3 Position;
		glm::vec3 Direction;
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

public:
	ParticleSystem(ParticleSystem&& other) = delete;
	ParticleSystem(const ParticleSystem& other) = delete;
	ParticleSystem& operator=(ParticleSystem&& other) = delete;
	ParticleSystem& operator=(const ParticleSystem& other) = delete;

	ParticleSystem();
	~ParticleSystem();

	virtual void Update(const Camera& camera, float deltaTime) override;

	void SetConeAngle(float angleRad) noexcept;
	void SetParticlesPerFrame(uint32 particlePerFrame) noexcept;
	void SetSpeed(float min, float max) noexcept;
	void SetTimeToLive(float timeToLive) noexcept;
	void SetTexture(uint32 textureID) noexcept;
	void SetColor(const glm::vec4& color) noexcept;
	uint32 GetNumParticles() const noexcept;

private:
	const Texture2D* GetTexture() const noexcept;
	const ParticleInstance* GetParticleInstances() const noexcept;
	const glm::vec4& GetColor() const noexcept;
	ParticleData& GetLivingParticle(uint32 index) noexcept;
	ParticleData& GetSortedParticle(uint32 index) noexcept;
	void InsertSortedParticle(uint32 id) noexcept;
	void SpawnParticle() noexcept;
	void KillParticle(uint32 index);

private:
	const Texture2D* m_pTexture;
	uint32* m_pLivingParticles;
	uint32* m_pSortedParticles;
	ParticleData* m_pParticles;
	ParticleInstance* m_pParticleInstances;
	
	glm::vec4 m_Direction;
	glm::vec4 m_Color;
	float m_TimeToLive;
	float m_MinSpeed;
	float m_MaxSpeed;
	float m_ConeAngle;
	uint32 m_NumSortedParticles;
	uint32 m_NumParticles;
	uint32 m_MaxParticles;
	uint32 m_ParticlesPerFrame;
};