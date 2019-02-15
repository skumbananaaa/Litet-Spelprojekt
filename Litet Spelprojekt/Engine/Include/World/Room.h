#pragma once
#include <EnginePch.h>

class API Room
{
public:
	Room(glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f)) noexcept;
	~Room();

	const glm::vec3& GetCenter() const noexcept;
	const bool IsActive() const noexcept;

	void SetActive(bool active) noexcept;

private:
	glm::vec3 m_Center;
	bool m_Active = false;

};