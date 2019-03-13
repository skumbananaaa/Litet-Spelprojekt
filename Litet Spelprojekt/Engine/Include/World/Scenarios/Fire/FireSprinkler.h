#pragma once

#include <EnginePch.h>
#include <Audio/Sources/AudioSource.h>
#include <Graphics/GameObject.h>

class API FireSprinkler : public GameObject
{
	static constexpr float SPRINKLER_RADIUS_SQRD = 25.0f;
	static constexpr float FIRE_EXTINGUISH_BY_SPRINKLER_RATE = 500.0f;
	static constexpr float WATER_SPRINKLED_PER_SECOND = 50.0f;
	static constexpr float WATER_RESERVOIR_MAX = 50.0f;
	static constexpr float WATER_RESERVOIR_RECOVERY_PER_SECOND = 50.0f;

public:
	FireSprinkler(int32 source);
	~FireSprinkler();

	virtual void Update(const Camera& camera, float dt) noexcept override;
	virtual void OnSmokeDetected() noexcept override;
	virtual void SetPosition(const glm::vec3& position) noexcept override;
	virtual bool HasDetectedSmoke() const noexcept override;
	void TurnOff() noexcept;

private:
	bool m_HasDetectedSmoke;
	float m_WaterReservoir;
	AudioSource* m_pAudioSrc;
};