#include <EnginePch.h>
#include <World/FireAlarm.h>

FireAlarm::FireAlarm(int32 source) : GameObject(), 
	m_FireDetected(false)
{
	//GameObject::SetMesh(MESH::LAMP);
	//GameObject::SetMaterial(MATERIAL::RED);
	m_Src = AudioSource::CreateSoundSource(source);
	m_Src->SetRollOffFactor(10.0f);
	m_Src->SetReferenceDistance(0.0f);
	m_Src->SetMaxDistance(500.0f);
	m_Src->SetLooping(true);
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
