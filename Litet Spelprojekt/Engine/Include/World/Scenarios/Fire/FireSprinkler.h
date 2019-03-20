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
	static constexpr float SPRINKLER_RESERVOIR_MAX = 1.0f;
	static constexpr float SPRINKLER_RESERVOIR_MAX_MINUS_EPSILON = 1.0f - 0.00001f;
	static constexpr float SPRINKLER_WATER_LOSS_PER_SECOND = 1.0f / (2.0f * 60.0f);
	static constexpr float SPRINKLER_WATER_GAIN_PER_SECOND = 1.0f / (3.0f * 60.0f);
	static constexpr float SPRINKLER_SECONDS_TO_TURN_OFF = 10.0f;

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
	bool IsInsideBoundingBox(const glm::ivec2& pos) const noexcept;

private:
	Scene* m_pScene;
	bool m_HasDetectedSmoke;
	AudioSource* m_pAudioSrc;
	MeshEmitter* m_pParticleEmitter;
	glm::vec3 m_MinBounds;
	glm::vec3 m_MaxBounds;
	float m_WaterReservoir;
	float m_TimeToTurnOff;
};

inline bool FireSprinkler::IsInsideBoundingBox(const glm::ivec2& pos) const noexcept
{
	if (m_MinBounds == m_MaxBounds)
	{
		return true;
	}

	return	pos.x >= m_MinBounds.x && pos.y >= m_MinBounds.z &&
			pos.x <= m_MaxBounds.x && pos.y <= m_MaxBounds.z;
}