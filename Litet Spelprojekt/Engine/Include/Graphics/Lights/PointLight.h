#pragma once
#include <EnginePch.h>

class API PointLight
{
public:
	PointLight(const glm::vec4& color, const glm::vec3& position);
	~PointLight();

	void SetColor(const glm::vec4& color) noexcept;
	void SetPosition(const glm::vec3& direction) noexcept;
	const glm::vec3& GetPosition() const noexcept;
	const glm::vec4& GetColor() const noexcept;

private:
	glm::vec4 m_Color;
	glm::vec3 m_Position;
};

inline void PointLight::SetColor(const glm::vec4& color) noexcept
{
	m_Color = color;
}

inline void PointLight::SetPosition(const glm::vec3& position) noexcept
{
	m_Position = position;
}

inline const glm::vec3& PointLight::GetPosition() const noexcept
{
	return m_Position;
}

inline const glm::vec4& PointLight::GetColor() const noexcept
{
	return m_Color;
}
