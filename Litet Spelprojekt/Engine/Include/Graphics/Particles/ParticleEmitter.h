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

	ParticleEmitter(const ParticleSystem* pSystem);
	~ParticleEmitter();

	virtual void Update(float deltaTime) noexcept override;

private:
	const ParticleSystem* m_pSystem;
};