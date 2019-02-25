#pragma once
#include <Graphics/GameObject.h>
#include "ParticleData.h"

enum ParticleBlendMode : uint32
{
	PARTICLE_NORMAL = 1,
	PARTICLE_ADDITIVE = 2
};

class API ParticleEmitter :  public GameObject
{
	friend class ForwardRenderer;
	friend class DefferedRenderer;

public:
	ParticleEmitter(ParticleEmitter&& other) = delete;
	ParticleEmitter(const ParticleEmitter& other) = delete;
	ParticleEmitter& operator=(ParticleEmitter&& other) = delete;
	ParticleEmitter& operator=(const ParticleEmitter& other) = delete;

	ParticleEmitter();
	~ParticleEmitter();

	virtual void Update(const Camera& camera, float deltaTime) noexcept override;
	virtual void UpdateTransform() noexcept override;
	
	void SetParticlesPerSeconds(uint32 numParticles) noexcept;
	void SetParticleBlendMode(ParticleBlendMode mode) noexcept;
	void SetConeAngle(float angleRad) noexcept;
	void SetScale(const glm::vec2& begin, const glm::vec2& end) noexcept;
	void SetSpeed(float min, float max) noexcept;
	void SetTimeToLive(float timeToLive) noexcept;
	void SetTexture(uint32 textureID) noexcept;
	void SetBeginColor(const glm::vec4& color) noexcept;
	void SetEndColor(const glm::vec4& color) noexcept;

	Node<glm::vec4>& GetColorNode(uint32 index) noexcept;
	const Node<glm::vec4>& GetColorNode(uint32 index) const noexcept;
	uint32 GetNumParticles() const noexcept;
	float GetDistToCamera() const noexcept;

	void AddColorNode(const glm::vec4& color, float atLifeTime) noexcept;

private:
	void SpawnParticle() noexcept;
	const Texture2D* GetTexture() const noexcept;
	const ParticleInstance* GetParticleInstances() const noexcept;
	ParticleData& GetLivingParticle(uint32 index) noexcept;
	ParticleData& GetSortedParticle(uint32 index) noexcept;
	void InsertSortedParticle(uint32 id) noexcept;
	void KillParticle(uint32 index);
	ParticleBlendMode GetParticleBlendMode() const noexcept;

private:
	const Texture2D* m_pTexture;
	float m_ParticleBacklog;
	uint32 m_ParticlesPerSecond;
	glm::vec3 m_Direction;
	uint32 m_LivingParticles[MAX_PARTICLES];
	uint32 m_SortedParticles[MAX_PARTICLES];
	ParticleData m_Particles[MAX_PARTICLES];
	ParticleInstance m_ParticleInstances[MAX_PARTICLES];
	ParticleBlendMode m_BlendMode;
	std::vector<Node<glm::vec4>> m_ColorNodes;
	glm::vec2 m_BeginScale;
	glm::vec2 m_EndScale;
	float m_TimeToLive;
	float m_MinSpeed;
	float m_MaxSpeed;
	float m_ConeAngle;
	float m_DistToCamera;
	uint32 m_NumSortedParticles;
	uint32 m_NumParticles;

public:
	static bool DistLess(ParticleEmitter* pEmitter1, ParticleEmitter* pEmitter2);
	static bool DistGreater(ParticleEmitter* pEmitter1, ParticleEmitter* pEmitter2);
};