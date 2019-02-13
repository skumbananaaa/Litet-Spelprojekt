#include "../Include/FireAlarm.h"

FireAlarm::FireAlarm(int32 source) : GameObject(), AudioSource(*AudioSource::CreateSoundSource(source)), m_FireDetected(false)
{
}

void FireAlarm::SetPosition(const glm::vec3 & position) noexcept
{
	GameObject::SetPosition(position);
	AudioSource::SetPosition(position);
}

void FireAlarm::SetDetection(bool fireDetected)
{
	m_FireDetected = fireDetected;

	if (m_FireDetected)
	{
		AudioSource::Play();
	}
	else
	{
		AudioSource::Stop();
	}
}

void FireAlarm::Update(float dt)
{
	GameObject::Update(dt);
}
