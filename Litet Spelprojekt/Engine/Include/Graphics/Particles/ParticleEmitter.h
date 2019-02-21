#pragma once
#include <Graphics/GameObject.h>

class ParticleSystem;

class API ParticleEmitter :  public GameObject
{
public:
	ParticleEmitter(ParticleEmitter&& other) = delete;
	ParticleEmitter(const ParticleEmitter& other) = delete;
	ParticleEmitter& operator=(ParticleEmitter&& other) = delete;
	ParticleEmitter& operator=(const ParticleEmitter& other) = delete;

	ParticleEmitter(ParticleSystem* pSystem);
	~ParticleEmitter();

	virtual void Update(float deltaTime) noexcept override;

	void SetParticlesPerFrame(int32 numParticles) noexcept;

private:
	ParticleSystem* m_pSystem;
	int32 m_ParticlesPerFrame;
};