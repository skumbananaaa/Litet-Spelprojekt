#include <Graphics/GUI/GUI.h>

GUI::GUI(float width, float height)
{
	FramebufferDesc desc;
	desc.DepthStencilFormat = TEX_FORMAT_UNKNOWN;
	desc.ColorAttchmentFormats[0] = TEX_FORMAT_RGBA;
	desc.SamplingParams = TextureParams();
	desc.NumColorAttachments = 1;
	desc.Width = width;
	desc.Height = height;

	this->m_pFramebuffer = new Framebuffer(desc);
}
