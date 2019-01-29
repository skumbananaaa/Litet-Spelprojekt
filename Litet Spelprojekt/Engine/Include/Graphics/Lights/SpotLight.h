#pragma once
#include <EnginePch.h>
#include "Graphics/GameObject.h"

class API SpotLight :
	public GameObject
{
public:
	SpotLight(const glm::vec3& pos, const float innerAngle, const float outerAngle, const glm::vec3& direction, const glm::vec4& color = glm::vec4(1.0f)) noexcept;
	~SpotLight();

	const glm::vec4& GetColor() const noexcept;
	const glm::vec3& GetDirection() const noexcept;
	const float GetCutOffAngle() const noexcept;
	const float GetOuterCutOffAngle() const noexcept;
	void  SetDirection(glm::vec3 target) noexcept;


public:

private:
	glm::vec4 m_Color;
	glm::vec3 m_Direction;
	float m_CutOffAngle; // radius of light
	float m_OuterCutOffAngle; // radius of light spread

};

inline const glm::vec4& SpotLight::GetColor() const noexcept
{
	return m_Color;
}

inline const glm::vec3& SpotLight::GetDirection() const noexcept
{
	return m_Direction;
}

inline const float SpotLight::GetCutOffAngle() const noexcept
{
	return m_CutOffAngle;
}

inline const float SpotLight::GetOuterCutOffAngle() const noexcept
{
	return m_OuterCutOffAngle;
}


inline void SpotLight::SetDirection(glm::vec3 target) noexcept
{
	m_Direction = target;
}