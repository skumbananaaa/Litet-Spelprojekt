#include <EnginePch.h>
#include <World/Room.h>

Room::Room(glm::vec3 center) noexcept
{
	m_Center = center;
}

Room::~Room()
{
}

const glm::vec3 & Room::GetCenter() const noexcept
{
	return m_Center;
}
