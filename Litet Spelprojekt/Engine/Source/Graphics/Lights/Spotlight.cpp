#include <EnginePch.h>
#include <Graphics/Lights/SpotLight.h>

SpotLight::SpotLight(const glm::vec3& pos, const float innerAngle, const float outerAngle, const glm::vec3& direction, const glm::vec4& color) noexcept :
	GameObject()
{
	SetPosition(pos);
	m_Color = color;
	m_Direction = direction;
	m_CutOffAngle = innerAngle;
	m_OuterCutOffAngle = outerAngle;
}


SpotLight::~SpotLight()
{
}