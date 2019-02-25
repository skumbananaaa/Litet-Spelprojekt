#include <EnginePch.h>
#include <World/Scenarios/Fire/FireAlarm.h>
#include <World/LightManager.h>
#include <Graphics/Lights/SpotLight.h>
#include <Graphics/Scene.h>
#include <World/Logger.h>

FireAlarm::FireAlarm(int32 source) : GameObject(),
	m_FireDetected(false),
	m_pSpotlight(nullptr)
{
	m_pAudioSrc = AudioSource::CreateSoundSource(source);
	m_pAudioSrc->SetRollOffFactor(10.0f);
	m_pAudioSrc->SetReferenceDistance(0.0f);
	m_pAudioSrc->SetMaxDistance(500.0f);
	m_pAudioSrc->SetLooping(true);
}

FireAlarm::~FireAlarm()
{
	DeleteSafe(m_pAudioSrc);
}

void FireAlarm::SetPosition(const glm::vec3& position) noexcept
{
	GameObject::SetPosition(position);
	m_pAudioSrc->SetPosition(position);
}

void FireAlarm::TurnOff() noexcept
{
	m_FireDetected = false;
	m_pSpotlight = nullptr;
	m_pAudioSrc->Stop();
}

void FireAlarm::Update(const Camera& camera, float dt) noexcept
{
	GameObject::Update(camera, dt);

	if (m_pSpotlight)
	{
		glm::mat4 transformObject(1.0f);
		glm::mat4 transformLight(1.0f);
		const glm::vec4& rotation = GetRotation();
		transformObject = glm::rotate(transformObject, rotation.w, glm::vec3(rotation.x, rotation.y, rotation.z));
		transformLight = glm::rotate(transformLight, m_Rotation, glm::vec3(1, 0, 0));

		glm::mat4 result = transformObject * transformLight;
		glm::vec4 dir = result * glm::vec4(0, 1, 0, 1);

		m_Rotation += dt * 10;
		m_pSpotlight->SetDirection(glm::vec3(dir.x, dir.y, dir.z));
	}
}

void FireAlarm::OnSmokeDetected() noexcept
{
	Logger::LogEvent("Smoke Detected!", true);

	glm::mat4 transformObject(1.0f);
	const glm::vec4& rotation = GetRotation();
	transformObject = glm::rotate(transformObject, rotation.w, glm::vec3(rotation.x, rotation.y, rotation.z));
	transformObject = glm::translate(transformObject, glm::vec3(-0.4F, 1.64, 0));
	glm::vec4 pos = transformObject * glm::vec4(0, 0, 0, 1);
	pos += glm::vec4(GetPosition(), 1);

	m_pSpotlight = LightManager::AcquireSpotlight(pos, glm::cos(glm::radians(20.5f)), glm::cos(glm::radians(40.5f)), glm::vec3(1, 0, 0), glm::vec4(1.0, 0.25, 0.0, 1.0));

	m_FireDetected = true;
	m_Rotation = 0;
	m_pAudioSrc->Play();
}