#include <EnginePch.h>
#include <World/Room.h>

Room::Room(Room&& other)
	: m_pShadowMap(other.m_pShadowMap),
	m_Center(other.m_Center),
	m_Initialized(other.m_Initialized)
{
	other.m_pShadowMap = nullptr;
	other.m_Center = glm::vec3(0.0f);
	other.m_Initialized = false;
}

Room::Room() noexcept
	: m_pShadowMap(nullptr),
	m_Center(glm::vec3(0.0f)),
	m_Initialized(false)
{
}

Room::~Room()
{
	DeleteSafe(m_pShadowMap);
}

void Room::Init(const glm::vec3& center) noexcept
{
	m_Center = center;
	m_Initialized = true;
}

void Room::SetActive(bool active) noexcept
{
	m_Active = active;
}

void Room::SetBurning(bool burning) noexcept
{
	m_Burning = burning;
}

void Room::SetFireDetected(bool detected) noexcept
{
	m_FireDetected = detected;
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

void Room::GenerateShadows(const Scene& scene) noexcept
{
	m_pShadowMap = new StaticShadowCube(m_Center, scene);
}
