#include <EnginePch.h>
#include <Graphics/Materials/Material.h>

Material::Material()
	: m_pProgram(nullptr),
	m_Data(),
	m_PipelineState()
{
	for (uint32 i = 0; i < NUM_CLIP_DISTANCES; i++)
	{
		m_PipelineState.ClipPlanesEnabled[i] = false;
	}
}

Material::Material(ShaderProgram* pProgram)
	: m_pProgram(pProgram),
	m_Data(),
	m_PipelineState()
{
	for (uint32 i = 0; i < NUM_CLIP_DISTANCES; i++)
	{
		m_PipelineState.ClipPlanesEnabled[i] = false;
	}
}

Material::~Material()
{
}

void Material::Bind(const Framebuffer* pGBuffer) const noexcept
{
	GLContext& context = GLContext::GetCurrentContext();

	context.SetProgram(m_pProgram);

	context.SetUniformBuffer(m_Data.pCameraBuffer, CAMERA_BUFFER_BINDING_SLOT);
	context.SetUniformBuffer(m_Data.pLightBuffer, LIGHT_BUFFER_BINDING_SLOT);
	context.SetUniformBuffer(m_Data.pMaterialBuffer, MATERIAL_BUFFER_BINDING_SLOT);

	context.SetTexture(m_Data.pDiffuseMap, DIFFUSE_MAP_BINDING_SLOT);
	context.SetTexture(m_Data.pNormalMap, NORMAL_MAP_BINDING_SLOT);
	context.SetTexture(m_Data.pSpecularMap, SPECULAR_MAP_BINDING_SLOT);
}

void Material::Unbind() const noexcept
{
	GLContext& context = GLContext::GetCurrentContext();

	context.SetProgram(nullptr);

	context.SetUniformBuffer(nullptr, CAMERA_BUFFER_BINDING_SLOT);
	context.SetUniformBuffer(nullptr, LIGHT_BUFFER_BINDING_SLOT);
	context.SetUniformBuffer(nullptr, MATERIAL_BUFFER_BINDING_SLOT);

	context.SetTexture(nullptr, DIFFUSE_MAP_BINDING_SLOT);
	context.SetTexture(nullptr, NORMAL_MAP_BINDING_SLOT);
	context.SetTexture(nullptr, SPECULAR_MAP_BINDING_SLOT);
}

void Material::SetLightBuffer(const UniformBuffer* pLightBuffer) const noexcept
{
	m_Data.pLightBuffer = pLightBuffer;
}

void Material::SetCameraBuffer(const UniformBuffer* pCameraBuffer) const noexcept
{
	m_Data.pCameraBuffer = pCameraBuffer;
}

void Material::SetMaterialBuffer(const UniformBuffer* pMaterialBuffer) const noexcept
{
	m_Data.pMaterialBuffer = pMaterialBuffer;
}
