#include <EnginePch.h>
#include <World/Room.h>

Room::Room(Room&& other)
	: m_pShadowMap(other.m_pShadowMap),
	m_Center(other.m_Center)
{
	other.m_pShadowMap = nullptr;
	other.m_Center = glm::vec3(0.0f);
}

Room::Room() noexcept
	: m_pShadowMap(nullptr),
	m_Center(glm::vec3(0.0f))
{
}

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

void Room::SetActive(bool active) noexcept
{
	m_Active = active;
}

void Room::GenerateShadows(const Scene& scene) noexcept
{
	m_pShadowMap = new StaticShadowCube(m_Center, scene);
}
