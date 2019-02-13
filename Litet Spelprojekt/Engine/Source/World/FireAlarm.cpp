#include <EnginePch.h>
#include <World/FireAlarm.h>

FireAlarm::FireAlarm(int32 source) : GameObject(), 
	m_FireDetected(false)
{
	GameObject::SetMesh(MESH::LAMP);
	m_Src = AudioSource::CreateSoundSource(source);
}

void FireAlarm::SetPosition(const glm::vec3 & position) noexcept
{
	GameObject::SetPosition(position);
	m_Src->SetPosition(position);
}

void FireAlarm::SetDetection(bool fireDetected)
{
	m_FireDetected = fireDetected;

	if (m_FireDetected)
	{
		m_Src->Play();
	}
	else
	{
		m_Src->Stop();
	}
}

void FireAlarm::Update(float dt)
{
	GameObject::Update(dt);
}
