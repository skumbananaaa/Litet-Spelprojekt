#include <EnginePch.h>
#include <Graphics/Materials/WaterOutdoorMaterial.h>
#include <Graphics/Textures/Texture2D.h>
#include <Graphics/Renderers/GLContext.h>

WaterOutdoorMaterial::WaterOutdoorMaterial() : Material(SHADER::WATER_OUTDOOR_MATERIAL),
	m_pReflector(nullptr),
	m_pDistortion(nullptr),
	m_pDepthMap(nullptr),
	m_pWaterBuffer(nullptr),
	m_Buffer()
{
	m_Buffer.WaveFactor = glm::vec2(0.0f);
	m_pWaterBuffer = new UniformBuffer(&m_Buffer, 1, sizeof(WaterOutdoorBuffer));
	SetIncludeInDepthPrePass(false);
	SetIsReflectable(true);
}

WaterOutdoorMaterial::~WaterOutdoorMaterial()
{
	DeleteSafe(m_pWaterBuffer);
}

void WaterOutdoorMaterial::Bind(const Framebuffer* pGBuffer) const noexcept
{
	GLContext& context = GLContext::GetCurrentContext();

	context.SetUniformBuffer(m_pWaterBuffer, 6);

	context.SetTexture(m_pDistortion, 3);
	context.SetTexture(m_pDisplacementMap, 6);

	if (m_pReflector)
	{
		context.SetTexture(m_pReflector->GetReflectionTexture(), 4);
	}

	m_LastDepthMask = context.GetDepthMask();
	m_LastDepthFunc = context.GetDepthFunc();

	context.Enable(DEPTH_TEST);
	context.SetDepthFunc(FUNC_LESS);
	context.SetDepthMask(true);

	Material::Bind(pGBuffer);
}

void WaterOutdoorMaterial::Unbind() const noexcept
{
	GLContext& context = GLContext::GetCurrentContext();

	context.SetUniformBuffer(nullptr, 6);

	context.SetTexture(nullptr, 3);
	context.SetTexture(nullptr, 4);
	context.SetTexture(nullptr, 5);
	context.SetTexture(nullptr, 6);

	context.SetDepthMask(m_LastDepthMask);
	context.SetDepthFunc(m_LastDepthFunc);
	Material::Unbind();
}

void WaterOutdoorMaterial::SetPlanarReflector(PlanarReflector* pReflector) const
{
	assert(pReflector != nullptr);
	m_pReflector = pReflector;
}

void WaterOutdoorMaterial::SetDistortionTexture(Texture2D* pDistortion)
{
	m_pDistortion = pDistortion;
}

void WaterOutdoorMaterial::SetDisplacementMap(Texture2D* pDisplacementMap)
{
	m_pDisplacementMap = pDisplacementMap;
}

void WaterOutdoorMaterial::SetWaveFactor(const glm::vec2& waveFactor) const
{
	m_Buffer.WaveFactor = waveFactor;
	m_pWaterBuffer->UpdateData(&m_Buffer);
}
