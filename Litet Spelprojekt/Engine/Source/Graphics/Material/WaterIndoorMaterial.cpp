#include <EnginePch.h>
#include <Graphics/Materials/WaterIndoorMaterial.h>
#include <Graphics/Textures/Texture2D.h>
#include <Graphics/Renderers/GLContext.h>

WaterIndoorMaterial::WaterIndoorMaterial() : Material(SHADER::WATER_INDOOR_MATERIAL),
m_pWaterBuffer(nullptr),
m_Buffer()
{
	m_Buffer.DistortionFactor = 0.0f;
	m_pWaterBuffer = new UniformBuffer(&m_Buffer, 1, sizeof(WaterIndoorBuffer));
}

WaterIndoorMaterial::~WaterIndoorMaterial()
{
	DeleteSafe(m_pWaterBuffer);
}

void WaterIndoorMaterial::Bind(const Framebuffer* pGBuffer) const noexcept
{
	GLContext& context = GLContext::GetCurrentContext();

	context.SetUniformBuffer(m_pWaterBuffer, 6);

	Material::Bind(pGBuffer);
}

void WaterIndoorMaterial::Unbind() const noexcept
{
	GLContext& context = GLContext::GetCurrentContext();

	context.SetUniformBuffer(nullptr, 6);

	Material::Unbind();
}

void WaterIndoorMaterial::SetDistortionFactor(float distortionFactor) const
{
	m_Buffer.DistortionFactor = distortionFactor;
	m_pWaterBuffer->UpdateData(&m_Buffer);
}
