#pragma once
#include <EnginePch.h>

class API DirectionalLight
{
public:
	DirectionalLight(const glm::vec4& color, const glm::vec3& direction);
	~DirectionalLight();

	void SetColor(const glm::vec4& color) noexcept;
	void SetDirection(const glm::vec3& direction) noexcept;
	const glm::vec3& GetDirection() const noexcept;
	const glm::vec4& GetColor() const noexcept;

private:
	glm::vec4 m_Color;
	glm::vec3 m_Direction;
};

inline void DirectionalLight::SetColor(const glm::vec4& color) noexcept
{
	m_Color = color;
}

inline void DirectionalLight::SetDirection(const glm::vec3& direction) noexcept
{
	m_Direction = direction;
}

inline const glm::vec3& DirectionalLight::GetDirection() const noexcept
{
	return m_Direction;
}

inline const glm::vec4& DirectionalLight::GetColor() const noexcept
{
	return m_Color;
}
