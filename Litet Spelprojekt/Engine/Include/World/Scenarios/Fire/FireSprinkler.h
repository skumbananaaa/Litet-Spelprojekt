#pragma once

#include <EnginePch.h>
#include <Audio/Sources/AudioSource.h>
#include <Graphics/GameObject.h>

class API FireSprinkler : public GameObject
{
public:
	FireSprinkler(int32 source);
	~FireSprinkler();

	virtual void Update(const Camera& camera, float dt) noexcept override;
	virtual void OnSmokeDetected() noexcept override;
	virtual void SetPosition(const glm::vec3& position) noexcept override;
	bool HasDetected() const noexcept;
	void TurnOff() noexcept;

private:
	AudioSource* m_pAudioSrc;
};