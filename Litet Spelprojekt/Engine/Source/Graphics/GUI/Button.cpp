#include <EnginePch.h>
#include <Graphics\GUI\Button.h>

Button::Button(float x, float y, float width, float height, const std::string& text, void(*onPressedCallback)(Button*), void(*onReleasedCallback)(Button*), int textSize) : TextView(x, y, width, height, text, textSize),
	m_pOnPressedTexture(nullptr),
	m_IsPressed(false),
	m_OnPressedCallback(onPressedCallback),
	m_OnReleasedCallback(onReleasedCallback)
{
	SetTextAlignment(CENTER);
}

Button::~Button()
{
}

Texture2D* Button::GetOnPressedTexture() const noexcept
{
	return m_pOnPressedTexture;
}

void Button::SetOnPressedTexture(Texture2D* texture)
{
	if (m_pOnPressedTexture != texture)
	{
		m_pOnPressedTexture = texture;
		RequestRepaint();
	}
}

void Button::SetOnButtonPressed(void(*callback)(Button*))
{
	m_OnPressedCallback = callback;
}

void Button::SetOnButtonCallback(void(*callback)(Button*))
{
	m_OnReleasedCallback = callback;
}

void Button::OnMousePressed(const glm::vec2& position, MouseButton mousebutton)
{
	if (ContainsPoint(position))
	{
		m_IsPressed = true;
		RequestRepaint();
		if (m_OnPressedCallback)
		{
			m_OnPressedCallback(this);
		}
	}
}

void Button::OnMouseReleased(const glm::vec2& position, MouseButton mousebutton)
{
	if (m_IsPressed)
	{
		m_IsPressed = false;
		RequestRepaint();
		if (m_OnReleasedCallback)
		{
			m_OnReleasedCallback(this);
		}
	}
}

void Button::OnMouseMove(const glm::vec2& lastPosition, const glm::vec2& position)
{

}

void Button::OnAdded(GUIObject* parent)
{
	AddMouseListener(this);
}

void Button::OnRemoved(GUIObject* parent)
{
	RemoveMouseListener(this);
}

void Button::RenderBackgroundTexture(GUIContext* context)
{
	context->SetVertexQuadData(0, 0, GetWidth(), GetHeight());
	if (GetTexture())
	{
		if (m_pOnPressedTexture)
		{
			if (m_IsPressed)
			{
				context->SetVertexQuadData(0, 0, GetWidth(), GetHeight());
				context->GetGraphicsContext()->SetTexture(m_pOnPressedTexture, 0);
				glDrawArrays(GL_TRIANGLES, 0, 6);
				context->GetGraphicsContext()->SetTexture(nullptr, 0);
				return;
			}
		}
		GUIObject::RenderBackgroundTexture(context);
	}
}