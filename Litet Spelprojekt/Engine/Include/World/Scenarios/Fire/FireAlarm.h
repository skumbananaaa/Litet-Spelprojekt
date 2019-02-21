#pragma once
#include <EnginePch.h>
#include <Audio/Sources/AudioSource.h>
#include <Graphics/GameObject.h>

class SpotLight;

class API FireAlarm : public GameObject
{
public:
	FireAlarm(int32 source);
	~FireAlarm();
	virtual void SetPosition(const glm::vec3& position) noexcept override;
	void TurnOff() noexcept;
	virtual void Update(float dt) noexcept override;
	virtual void OnSmokeDetected() noexcept override;

private:
	bool m_FireDetected;
	float m_Rotation;
	AudioSource* m_pAudioSrc;
	SpotLight* m_pSpotlight;
};