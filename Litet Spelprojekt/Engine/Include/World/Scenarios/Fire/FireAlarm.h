#pragma once
#include <EnginePch.h>
#include <Audio/Sources/AudioSource.h>
#include <Graphics/GameObject.h>

class API FireAlarm : public GameObject
{
public:
	FireAlarm(int32 source);
	void SetPosition(const glm::vec3& position) noexcept;
	void SetDetection(bool fireDetected);
	void Update(float dt) noexcept;
private:
	bool m_FireDetected;
	AudioSource* m_Src;
};