#include <EnginePch.h>
#include <Graphics/Lights/PointLight.h>

PointLight::PointLight(const glm::vec3& pos, const glm::vec4& color) noexcept :
	GameObject()
{
	SetPosition(pos);
	m_Color = color;
}


PointLight::~PointLight()
{
}


