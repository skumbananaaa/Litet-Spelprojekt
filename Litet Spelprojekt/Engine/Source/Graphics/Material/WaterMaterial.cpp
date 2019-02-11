#include <EnginePch.h>
#include <Graphics/Materials/WaterMaterial.h>
#include <Graphics/Textures/Texture2D.h>
#include <Graphics/Renderers/GLContext.h>

WaterMaterial::WaterMaterial() : Material(SHADER::DEFERRED_WATER),
	m_pReflector(nullptr),
	m_pDistortion(nullptr),
	m_pDepthMap(nullptr),
	m_pWaterBuffer(nullptr),
	m_Buffer()
{
	m_Buffer.DistortionFactor = 0.0f;
	m_pWaterBuffer = new UniformBuffer(&m_Buffer, 1, sizeof(WaterBuffer));
}

WaterMaterial::~WaterMaterial()
{
	DeleteSafe(m_pWaterBuffer);
}

void WaterMaterial::Bind(const Framebuffer* pGBuffer) const noexcept
{
	GLContext& context = GLContext::GetCurrentContext();

	context.SetUniformBuffer(m_pWaterBuffer, 3);

	context.SetTexture(m_pDistortion, 3);
	if (m_pReflector)
	{
		context.SetTexture(m_pReflector->GetReflectionTexture(), 4);
	}
	context.SetTexture(pGBuffer->GetDepthAttachment(), 5);

	Material::Bind(pGBuffer);
}

void WaterMaterial::Unbind() const noexcept
{
	GLContext& context = GLContext::GetCurrentContext();

	context.SetUniformBuffer(nullptr, 3);

	context.SetTexture(nullptr, 3);
	context.SetTexture(nullptr, 4);
	context.SetTexture(nullptr, 5);

	Material::Unbind();
}

void WaterMaterial::SetPlanarReflector(PlanarReflector* pReflector) const
{
	assert(pReflector != nullptr);
	m_pReflector = pReflector;
}

void WaterMaterial::SetDistortionTexture(Texture2D* pDistortion)
{
	m_pDistortion = pDistortion;
}

void WaterMaterial::SetDistortionFactor(float distortionFactor) const
{
	m_Buffer.DistortionFactor = distortionFactor;
	m_pWaterBuffer->UpdateData(&m_Buffer);
}
