#include <EnginePch.h>
#include <Graphics/Renderers/DefferedRenderer.h>
#include <System/Window.h>

DefferedRenderer::DefferedRenderer()
	: m_pGBuffer(nullptr)
{
	Create();
}

DefferedRenderer::~DefferedRenderer()
{
	if (m_pGBuffer != nullptr)
	{
		delete m_pGBuffer;
		m_pGBuffer = nullptr;
	}
}

void DefferedRenderer::DrawScene(const Scene& scene) const
{
}

void DefferedRenderer::Create()
{
	std::cout << "Createing deffered renderer" << std::endl;

	//We can destroy desc when gbuffer is created
	{
		FramebufferDesc desc = {};
		desc.ColorAttchmentFormats[0] = TEX_FORMAT_RGBA;
		desc.ColorAttchmentFormats[1] = TEX_FORMAT_RGBA16F;
		desc.ColorAttchmentFormats[2] = TEX_FORMAT_RGBA16F;
		desc.NumColorAttachments = 3;
		desc.DepthStencilFormat = TEX_FORMAT_DEPTH_STENCIL;
		desc.Width = Window::GetCurrentWindow().GetWidth();
		desc.Height = Window::GetCurrentWindow().GetHeight();

		m_pGBuffer = new Framebuffer(desc);
	}
}

void DefferedRenderer::GeometryPass(const Scene& scene)
{
}

void DefferedRenderer::LightPass(const Scene& scene)
{
}
