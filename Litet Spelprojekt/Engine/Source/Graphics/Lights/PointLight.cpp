#include <EnginePch.h>
#include <Graphics/Lights/PointLight.h>

PointLight::PointLight(const glm::vec4& color, const glm::vec3& position)
	: m_Position(position),
	m_Color(color)
{
}

PointLight::~PointLight()
{
}