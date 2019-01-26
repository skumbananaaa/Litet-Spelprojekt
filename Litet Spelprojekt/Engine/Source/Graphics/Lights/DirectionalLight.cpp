#include <EnginePch.h>
#include <Graphics/Lights/DirectionalLight.h>

DirectionalLight::DirectionalLight(const glm::vec4& color, const glm::vec3& direction)
	: m_Color(color),
	m_Direction(direction)
{
}

DirectionalLight::~DirectionalLight()
{
}
