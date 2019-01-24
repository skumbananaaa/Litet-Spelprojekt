#include <Graphics/GUI/GUIObject.h>
#include <Graphics/GUI/GUIManager.h>

GUIObject::GUIObject(float width, float height) : m_GUIManager(nullptr)
{
	FramebufferDesc desc;
	desc.DepthStencilFormat = TEX_FORMAT_UNKNOWN;
	desc.ColorAttchmentFormats[0] = TEX_FORMAT_RGBA;
	desc.SamplingParams = TextureParams();
	desc.NumColorAttachments = 1;
	desc.Width = width;
	desc.Height = height;

	m_pFramebuffer = new Framebuffer(desc);
}

float GUIObject::GetWidth() const noexcept
{
	return m_pFramebuffer->GetWidth();
}

float GUIObject::GetHeight() const noexcept
{
	return m_pFramebuffer->GetHeight();
}

float GUIObject::GetX() const noexcept
{
	return 0;
}

float GUIObject::GetY() const noexcept
{
	return 0;
}

int32 GUIObject::GetDepth() const noexcept
{
	return m_Depth;
}

void GUIObject::SetDepth(int32 depth) noexcept
{
	if (m_Depth != depth)
	{
		m_Depth = depth;
		m_GUIManager->MarkDepthDirty();
	}
}

bool GUIObject::IsDirty() const noexcept
{
	return m_IsDirty;
}

void GUIObject::OnAdded(GUIManager* guiManager)
{
	m_GUIManager = guiManager;
}

void GUIObject::OnRemoved(GUIManager* m_GUIManager)
{
	m_GUIManager = nullptr;
}

void GUIObject::OnUpdate(float dtS)
{
	
}

void GUIObject::OnRender(GLContext* context)
{
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void GUIObject::OnMousePressed(MouseButton mousebutton)
{

}

void GUIObject::OnMouseReleased(MouseButton mousebutton)
{

}

void GUIObject::OnMouseMove(const glm::vec2& position)
{

}

void GUIObject::OnKeyUp(KEY keycode)
{

}

void GUIObject::OnKeyDown(KEY keycode)
{

}

void GUIObject::InternalOnRender(GLContext* context)
{
	context->SetFramebuffer(m_pFramebuffer);
	context->SetClearColor(0.0, 0.0, 0.0, 0.0);
	context->Clear(CLEAR_FLAG_COLOR);
	glm::vec4 currentViewPort = context->GetViewPort();
	context->SetViewport(GetWidth(), GetHeight(), 0, 0);

	this->OnRender(context);

	context->SetFramebuffer(nullptr);
	context->SetViewport(currentViewPort);
}