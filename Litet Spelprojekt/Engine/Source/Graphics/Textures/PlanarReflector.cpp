#include <EnginePch.h>
#include <Graphics/Textures/PlanarReflector.h>

PlanarReflector::PlanarReflector(const glm::vec3& normal, float distFromOrigin) noexcept
	: m_pFramebuffer(nullptr),
	m_ClipPlane(glm::vec4(normal, distFromOrigin))
{
	Create();
}

PlanarReflector::~PlanarReflector()
{
	DeleteSafe(m_pFramebuffer);
}

void PlanarReflector::SetPlane(const glm::vec3& normal, float distFromOrigin) noexcept
{
	m_ClipPlane = glm::vec4(normal, distFromOrigin);
}

void PlanarReflector::Create() noexcept
{
	{
		TextureParams params = {};
		params.Wrap = TEX_PARAM_REPEAT;
		params.MinFilter = TEX_PARAM_LINEAR;
		params.MagFilter = TEX_PARAM_LINEAR;

		FramebufferDesc desc = {};
		desc.Height = REFLECTION_SIZE;
		desc.Width = REFLECTION_SIZE;
		desc.ColorAttchmentFormats[0] = TEX_FORMAT_RGBA;
		desc.SamplingParams = params;
		desc.NumColorAttachments = 1;
		desc.DepthStencilFormat = TEX_FORMAT_DEPTH;
		desc.Samples = 1;

		m_pFramebuffer = new Framebuffer(desc);
	}
}

Framebuffer* PlanarReflector::GetFramebuffer() const noexcept
{
	return m_pFramebuffer;
}

Texture2D* PlanarReflector::GetReflectionTexture() const noexcept
{
	return (Texture2D*)m_pFramebuffer->GetColorAttachment(0);
}

const glm::vec4& PlanarReflector::GetLevelClipPlane() const noexcept
{
	return m_ClipPlane;
}
