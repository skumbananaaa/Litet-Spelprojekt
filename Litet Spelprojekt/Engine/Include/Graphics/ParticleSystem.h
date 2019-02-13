#pragma once
#include "GameObject.h"
#include "Geometry/Particle.h"

class API ParticleSystem : public GameObject
{
	friend class DefferedRenderer;

private:
	struct ParticleInternal
	{
		glm::vec3 Position;
		glm::vec3 Speed;
		float LifeTime;
	};

public:
	ParticleSystem(ParticleSystem&& other) = delete;
	ParticleSystem(const ParticleSystem& other) = delete;
	ParticleSystem& operator=(ParticleSystem&& other) = delete;
	ParticleSystem& operator=(const ParticleSystem& other) = delete;

	ParticleSystem();
	~ParticleSystem();

	virtual void Update(float deltaTime) override;

	void SetTexture(uint32 textureID) noexcept;
	void SetColor(const glm::vec4& color) noexcept;
	uint32 GetNumParticles() const noexcept;

private:
	const Texture2D* GetTexture() const noexcept;
	const ParticleInstance* GetParticleInstances() const noexcept;
	const glm::vec4& GetColor() const noexcept;

private:
	const Texture2D* m_pTexture;
	ParticleInternal* m_pParticles;
	ParticleInstance* m_pParticleInstances;
	glm::vec4 m_Color;
	uint32 m_NumParticles;
};