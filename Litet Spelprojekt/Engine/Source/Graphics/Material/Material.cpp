#include <EnginePch.h>
#include <Graphics/Materials/Material.h>

Material::Material()
	: m_pProgram(nullptr),
	m_Data(),
	m_PipelineState()
{
}

Material::Material(int32 shader)
	: m_pProgram(nullptr),
	m_Data(),
	m_PipelineState()
{
	SetProgram(shader);
}

Material::~Material()
{
}

void Material::Bind(const Framebuffer* pGBuffer) const noexcept
{
	GLContext& context = GLContext::GetCurrentContext();

	context.SetProgram(m_pProgram);

	if (m_PipelineState.StencilTest)
	{
		context.Enable(STENCIL_TEST);
		context.SetStencilOpFrontFace(m_PipelineState.Front.StencilFail, m_PipelineState.Front.DepthFail, m_PipelineState.Front.DepthPass);
		context.SetStencilOpBackFace(m_PipelineState.Back.StencilFail, m_PipelineState.Back.DepthFail, m_PipelineState.Back.DepthPass);
		context.SetStencilFunc(m_PipelineState.StencilFunc, m_PipelineState.StencilRef, m_PipelineState.StencilValue);
		context.SetStencilMask(m_PipelineState.StencilMask);
	}
	else
	{
		context.Disable(STENCIL_TEST);
	}

	if (m_PipelineState.CullMode != CULL_MODE_NONE)
	{
		context.Enable(CULL_FACE);
		context.SetCullMode(m_PipelineState.CullMode);
	}
	else
	{
		context.Disable(CULL_FACE);
	}

	context.Enable(CLIP_DISTANCE0);

	context.SetUniformBuffer(m_Data.pCameraBuffer, CAMERA_BUFFER_BINDING_SLOT);
	context.SetUniformBuffer(m_Data.pLightBuffer, LIGHT_BUFFER_BINDING_SLOT);
	context.SetUniformBuffer(m_Data.pMaterialBuffer, MATERIAL_BUFFER_BINDING_SLOT);
	context.SetUniformBuffer(m_Data.pWorldBuffer, WORLD_BUFFER_BINDING_SLOT);
	context.SetUniformBuffer(m_Data.pExtensionBuffer, EXTENSION_BUFFER_BINDING_SLOT);

	context.SetTexture(m_Data.pDiffuseMap, DIFFUSE_MAP_BINDING_SLOT);
	context.SetTexture(m_Data.pSpecularMap, SPECULAR_MAP_BINDING_SLOT);
}

void Material::Unbind() const noexcept
{
	GLContext& context = GLContext::GetCurrentContext();

	context.Disable(CLIP_DISTANCE0);
	context.Disable(STENCIL_TEST);

	context.SetUniformBuffer(nullptr, CAMERA_BUFFER_BINDING_SLOT);
	context.SetUniformBuffer(nullptr, LIGHT_BUFFER_BINDING_SLOT);
	context.SetUniformBuffer(nullptr, MATERIAL_BUFFER_BINDING_SLOT);
	context.SetUniformBuffer(nullptr, WORLD_BUFFER_BINDING_SLOT);
	context.SetUniformBuffer(nullptr, EXTENSION_BUFFER_BINDING_SLOT);

	context.SetTexture(nullptr, DIFFUSE_MAP_BINDING_SLOT);
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

void Material::SetWorldBuffer(const UniformBuffer* pWorldBuffer) const noexcept
{
	m_Data.pWorldBuffer = pWorldBuffer;
}

void Material::SetExtensionBuffer(const UniformBuffer * pExtensionBuffer) const noexcept
{
	m_Data.pExtensionBuffer = pExtensionBuffer;
}
