#include <EnginePch.h>
#include <Graphics/Materials/WallMaterial.h>

WallMaterial::WallMaterial() : Material(SHADER::WALL_MATERIAL),
	m_pDissolveBuffer(nullptr)
{
	m_pDissolveBuffer = new UniformBuffer(&m_Buffer, 1, sizeof(DissolveBuffer));

	SetCullMode(CULL_MODE_NONE);
}

WallMaterial::~WallMaterial()
{
	DeleteSafe(m_pDissolveBuffer);
}

void WallMaterial::Bind(const Framebuffer* pGBuffer) const noexcept
{
	GLContext& context = GLContext::GetCurrentContext();

	context.Enable(CLIP_DISTANCE0);
	context.Enable(CLIP_DISTANCE1);
	context.Enable(CLIP_DISTANCE2);

	context.SetUniformBuffer(m_pDissolveBuffer, 3);

	Material::Bind(pGBuffer);
}

void WallMaterial::Unbind() const noexcept
{
	GLContext& context = GLContext::GetCurrentContext();

	context.Disable(CLIP_DISTANCE0);
	context.Disable(CLIP_DISTANCE1);
	context.Disable(CLIP_DISTANCE2);

	context.SetUniformBuffer(nullptr, 3);

	Material::Unbind();
}

void WallMaterial::SetClipPlane(const glm::vec4& plane, uint32 index) const noexcept
{
	m_Buffer.g_ClipDistances[index] = plane;
	m_pDissolveBuffer->UpdateData(&m_Buffer);
}
