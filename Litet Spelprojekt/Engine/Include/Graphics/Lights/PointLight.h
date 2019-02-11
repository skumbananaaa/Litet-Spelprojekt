#pragma once
#include <EnginePch.h>
#include "Graphics/GameObject.h"

class API PointLight :
	public GameObject
{
public:
	PointLight(const glm::vec3& pos, const glm::vec4& color = glm::vec4(1.0f)) noexcept;
	~PointLight();

	const glm::vec4& GetColor() const noexcept;
	void SetColor(const glm::vec4& color) noexcept;

public:

private:
	glm::vec4 m_Color;

};

inline const glm::vec4& PointLight::GetColor() const noexcept
{
	return m_Color;
}