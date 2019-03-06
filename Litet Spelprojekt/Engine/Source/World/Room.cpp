#include <EnginePch.h>
#include <World/Room.h>

Room::Room(glm::vec3 center) noexcept
	: m_pShadowMap(nullptr),
	m_Center(center)
{
}

Room::~Room()
{
	DeleteSafe(m_pShadowMap);
}

const StaticShadowCube* Room::GetShadowMap() const noexcept
{
	return m_pShadowMap;
}

const glm::vec3& Room::GetCenter() const noexcept
{
	return m_Center;
}

const bool Room::IsActive() const noexcept
{
	return m_Active;
}

const bool Room::IsBurning() const noexcept
{
	return m_Burning;
}

const bool Room::IsFireDetected() const noexcept
{
	return m_FireDetected;
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

void Room::GenerateShadows(const Scene& scene) noexcept
{
	m_pShadowMap = new StaticShadowCube(m_Center, scene);
}
