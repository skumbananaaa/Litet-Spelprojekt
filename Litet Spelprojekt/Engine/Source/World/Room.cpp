#include <EnginePch.h>
#include <World/Room.h>

Room::Room(Room&& other)
	: m_pShadowMap(other.m_pShadowMap),
	m_pAudioSourceFire(other.m_pAudioSourceFire),
	m_Center(other.m_Center),
	m_Initialized(other.m_Initialized)
{
	other.m_pShadowMap = nullptr;
	other.m_Center = glm::vec3(0.0f);
	other.m_Initialized = false;
}

Room::Room() noexcept
	: m_pShadowMap(nullptr),
	m_pAudioSourceFire(nullptr),
	m_Center(glm::vec3(0.0f)),
	m_Initialized(false)
{
}

Room::~Room()
{
	if (m_pAudioSourceFire)
	{
		m_pAudioSourceFire->Stop();
	}
	DeleteSafe(m_pAudioSourceFire);
	DeleteSafe(m_pShadowMap);
	m_OnFire.clear();
}

void Room::Init(const glm::vec3& center) noexcept
{
	m_Center = center;
	m_Initialized = true;
	
	m_pAudioSourceFire = AudioSource::CreateSoundSource(SOUND::MONO_FIRE);
	m_pAudioSourceFire->SetPosition(m_Center);
	m_pAudioSourceFire->SetRollOffFactor(10.0f);
	m_pAudioSourceFire->SetReferenceDistance(1.0f);
	m_pAudioSourceFire->SetMaxDistance(100.0f);
	m_pAudioSourceFire->SetLooping(true);
}

void Room::SetActive(bool active) noexcept
{
	m_Active = active;
}

void Room::SetFireDetected(bool detected) noexcept
{
	m_FireDetected = detected;

	if (detected && !m_pAudioSourceFire->IsPlaying())
	{
		m_pAudioSourceFire->Play();
	}
	else if (!detected && m_pAudioSourceFire->IsPlaying())
	{
		m_pAudioSourceFire->Stop();
	}
}

void Room::SetFlooded(bool flooded) noexcept
{
	m_Flooded = flooded;
}

void Room::SetFloodDetected(bool detected) noexcept
{
	m_FloodDetected = detected;
}

void Room::SetFloodUpdated(bool updated) noexcept
{
	m_FloodUpdatedThisFrame = updated;
}

void Room::SetPumping(bool pumping) noexcept
{
	m_Pumping = pumping;
}

void Room::GenerateShadows(const Scene& scene) noexcept
{
	m_pShadowMap = new StaticShadowCube(m_Center, scene);
}

void Room::ExtendAudioPos(float extension) noexcept
{
	m_pAudioSourceFire->SetPosition(m_Center + glm::vec3(extension * glm::floor(m_Center.y / 2), 0.0f, 0.0f));
}

void Room::SetTileOnFire(const glm::ivec3 & tile) noexcept
{
	if (std::find(m_OnFire.begin(), m_OnFire.end(), tile) == m_OnFire.end())
	{
		m_OnFire.push_back(tile);
	}
}

void Room::RemoveTileOnFire(const glm::ivec3 & tile) noexcept
{
	m_OnFire.erase(std::remove(m_OnFire.begin(), m_OnFire.end(), tile), m_OnFire.end());
	if (m_OnFire.empty())
	{
		SetFireDetected(false);
	}
}
