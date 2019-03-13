#pragma once

#include <EnginePch.h>
#include <Audio/Sources/AudioSource.h>
#include <Graphics/Particles/MeshEmitter.h>
#include <Graphics/GameObject.h>
#include <World/Scenarios/Fire/FireAlarm.h>

class API FireSprinkler : public GameObject
{
	static constexpr float SPRINKLER_RADIUS = 5.0f;
	static constexpr float SPRINKLER_RADIUS_SQRD = SPRINKLER_RADIUS * SPRINKLER_RADIUS;
	static constexpr float FIRE_EXTINGUISH_BY_SPRINKLER_RATE = 500.0f * RATE_OF_FIRE_SPREAD;

public:
	FireSprinkler(int32 source);
	~FireSprinkler();

	virtual void Update(const Camera& camera, float dt) noexcept override;
	virtual void OnSmokeDetected() noexcept override;
	virtual void SetPosition(const glm::vec3& position) noexcept override;
	virtual bool HasDetectedSmoke() const noexcept override;
	void TurnOff() noexcept;

	virtual void OnAddedToScene(Scene* pScene) noexcept override;

private:
	Scene* m_pScene;
	bool m_HasDetectedSmoke;
	AudioSource* m_pAudioSrc;
	MeshEmitter* m_pParticleEmitter;
};