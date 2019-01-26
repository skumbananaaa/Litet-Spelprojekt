#include <EnginePch.h>
#include <Graphics/GUI/GUIObject.h>
#include <Graphics/GUI/GUIManager.h>

GUIObject::GUIObject(float x, float y, float width, float height) :
	m_GUIManager(nullptr),
	m_pBackgroundTexture(nullptr)
{
	FramebufferDesc desc;
	desc.DepthStencilFormat = TEX_FORMAT_UNKNOWN;
	desc.ColorAttchmentFormats[0] = TEX_FORMAT_RGBA;
	desc.SamplingParams = TextureParams();
	desc.NumColorAttachments = 1;
	desc.Width = static_cast<uint32>(width);
	desc.Height = static_cast<uint32>(height);

	m_position.x = x;
	m_position.y = y;

	m_pFramebuffer = new Framebuffer(desc);
}

GUIObject::~GUIObject()
{
	delete m_pFramebuffer;
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
	return m_position.x;
}

float GUIObject::GetY() const noexcept
{
	return m_position.y;
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

Texture2D* GUIObject::GetTexture() const noexcept
{
	return m_pBackgroundTexture;
}

void GUIObject::SetTexture(Texture2D* texture)
{
	m_pBackgroundTexture = texture;
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

void GUIObject::OnRender(GLContext* context, FontRenderer* fontRenderer)
{
	if (m_pBackgroundTexture)
	{
		context->SetTexture(m_pBackgroundTexture, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		context->SetTexture(nullptr, 0);
	}
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

void GUIObject::RequestRepaint()
{
	if (m_GUIManager)
	{
		m_GUIManager->RequestRepaint(this);
	}
}