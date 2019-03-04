#include <EnginePch.h>
#include <Graphics\GUI\Button.h>

Button::Button(float x, float y, float width, float height, const std::string& text, void(*onPressedCallback)(Button*), void(*onReleasedCallback)(Button*), int textSize) : TextView(x, y, width, height, text, true, textSize),
	m_pOnPressedTexture(nullptr),
	m_pFramebufferText(nullptr),
	m_IsPressed(false),
	m_IsTextBuffered(false),
	m_PressedColor(GUIContext::COLOR_PRESSED),
	m_HoverColor(GUIContext::COLOR_HOVER),
	m_SelectedColor(GUIContext::COLOR_SELECTED),
	m_PressedTextColor(GUIContext::COLOR_TEXT_PRESSED),
	m_HoverTextColor(GUIContext::COLOR_TEXT_HOVER),
	m_SelectedTextColor(GUIContext::COLOR_TEXT_SELECTED),
	m_OnPressedCallback(onPressedCallback),
	m_OnReleasedCallback(onReleasedCallback)
{
	RecreateFrameBuffer(width, height);
	SetBackgroundColor(GUIContext::COLOR_BUTTON_BACKGROUND);
}

Button::~Button()
{
	m_ButtonListeners.clear();
	DeleteSafe(m_pFramebufferText);
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

const glm::vec4& Button::GetOnPressedColor() const noexcept
{
	return m_PressedColor;
}

void Button::SetOnPressedColor(const glm::vec4& color)
{
	if (m_PressedColor != color)
	{
		m_PressedColor = color;
		if (m_IsPressed)
		{
			RequestRepaint();
		}
	}
}

const glm::vec4& Button::GetOnHoverColor() const noexcept
{
	return m_HoverColor;
}

void Button::SetOnHoverColor(const glm::vec4& color)
{
	if (m_HoverColor != color)
	{
		m_HoverColor = color;
		if (m_IsHovered)
		{
			RequestRepaint();
		}
	}
}

const glm::vec4& Button::GetOnSelectedColor() const noexcept
{
	return m_SelectedColor;
}

void Button::SetOnSelectedColor(const glm::vec4& color)
{
	if (m_SelectedColor != color)
	{
		m_SelectedColor = color;
		if (IsSelected())
		{
			RequestRepaint();
		}
	}
}

const glm::vec4& Button::GetOnPressedTextColor() const noexcept
{
	return m_PressedTextColor;
}

void Button::SetOnPressedTextColor(const glm::vec4& color)
{
	if (m_PressedTextColor != color)
	{
		m_PressedTextColor = color;
		if (m_IsHovered)
		{
			RequestRepaint();
		}
	}
}

const glm::vec4& Button::GetOnHoverTextColor() const noexcept
{
	return m_HoverTextColor;
}

void Button::SetOnHoverTextColor(const glm::vec4& color)
{
	if (m_HoverTextColor != color)
	{
		m_HoverTextColor = color;
		if (m_IsHovered)
		{
			RequestRepaint();
		}
	}
}

const glm::vec4& Button::GetOnSelectedTextColor() const noexcept
{
	return m_SelectedTextColor;
}

void Button::SetOnSelectedTextColor(const glm::vec4& color)
{
	if (m_SelectedTextColor != color)
	{
		m_SelectedTextColor = color;
		if (IsSelected())
		{
			RequestRepaint();
		}
	}
}

void Button::SetSelected(bool selected)
{
	if (IsSelected() != selected)
	{
		ISelectable::SetSelected(selected);
		RequestRepaint();
	}
}

void Button::SetHovered(bool hovered)
{
	if (IsHovered() != hovered)
	{
		IHoverable::SetHovered(hovered);
		RequestRepaint();
	}
}

void Button::SetTextBuffered(bool buffered) noexcept
{
	if (m_IsTextBuffered != buffered)
	{
		m_IsTextBuffered = buffered;
		RequestRepaint();
		if (m_IsTextBuffered)
		{
			RecreateFrameBuffer(GetWidth(), GetHeight());
		}
		else
		{
			DeleteSafe(m_pFramebufferText);
		}
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

void Button::AddButtonListener(IButtonListener* listener)
{
	if (!Contains<IButtonListener>(m_ButtonListeners, listener))
	{
		m_ButtonListeners.push_back(listener);
	}
	else
	{
		std::cout << "ButtonListener already added" << std::endl;
	}
}

void Button::RemoveButtonListener(IButtonListener* listener)
{
	int32 counter = 0;
	for (IButtonListener* object : m_ButtonListeners)
	{
		if (object == listener)
		{
			m_ButtonListeners.erase(m_ButtonListeners.begin() + counter);
			return;
		}
		counter++;
	}
}

void Button::OnPressed(const glm::vec2 & position, MouseButton mousebutton) noexcept
{
	if (m_OnPressedCallback)
	{
		m_OnPressedCallback(this);
	}
	for (IButtonListener* listener : m_ButtonListeners)
	{
		listener->OnButtonPressed(this);
	}
}

void Button::OnReleased(const glm::vec2 & position, MouseButton mousebutton) noexcept
{
	if (m_OnReleasedCallback)
	{
		m_OnReleasedCallback(this);
	}
	for (IButtonListener* listener : m_ButtonListeners)
	{
		listener->OnButtonReleased(this);
	}
	TriggerOnSelected(this);
}

void Button::OnMousePressed(const glm::vec2& position, MouseButton mousebutton)
{
	if (ContainsPoint(position))
	{
		m_IsPressed = true;
		RequestRepaint();
		OnPressed(position, mousebutton);
	}
}

void Button::OnMouseReleased(const glm::vec2& position, MouseButton mousebutton)
{
	if (m_IsPressed)
	{
		m_IsPressed = false;
		RequestRepaint();
		OnReleased(position, mousebutton);
	}
}

void Button::OnMouseMove(const glm::vec2& position)
{
	if (ContainsPoint(position))
	{
		if (!m_IsHovered)
		{
			m_IsHovered = true;
			RequestRepaint();
			for (IButtonListener* listener : m_ButtonListeners)
			{
				listener->OnButtonHovered(this);
			}
		}
	}
	else if (m_IsHovered)
	{
		m_IsHovered = false;
		RequestRepaint();
		for (IButtonListener* listener : m_ButtonListeners)
		{
			listener->OnButtonNotHovered(this);
		}
	}
	TriggerSendUpdate(this);
}

void Button::OnAdded(GUIObject* parent)
{
	AddMouseListener(this);
}

void Button::OnRemoved(GUIObject* parent)
{
	RemoveMouseListener(this);
}

void Button::PrintName() const
{
	std::cout << "Button";
}

const glm::vec4& Button::GetClearColor() const
{
	if (m_IsPressed)
	{
		return GetOnPressedColor();
	}
	else if (IsSelected())
	{
		return GetOnSelectedColor();
	}
	else if (m_IsHovered)
	{
		return GetOnHoverColor();
	}
	return TextView::GetClearColor();
}

Texture2D* Button::GetClearTexture() const
{
	if (m_IsPressed)
	{
		return GetOnPressedTexture();
	}
	return TextView::GetClearTexture();
}

const glm::vec4& Button::GetClearTextColor() const
{
	if (m_IsPressed)
	{
		return GetOnPressedTextColor();
	}
	else if (IsSelected())
	{
		return GetOnSelectedTextColor();
	}
	else if (m_IsHovered)
	{
		return GetOnHoverTextColor();
	}
	return TextView::GetClearTextColor();
}

void Button::OnPreRender(GUIContext* context)
{
	if (m_IsTextBuffered)
	{
		context->BeginSelfRendering(m_pFramebufferText, GUIContext::COLOR_TRANSPARENT);
		RenderText(context);
	}
}

void Button::RenderTextBuffered(GUIContext* context, float x, float y)
{
	context->RenderFrameBuffer(m_pFramebufferText, x, y);
}

void Button::OnRender(GUIContext* context)
{
	if (m_IsTextBuffered)
	{
		GUIObject::OnRender(context);
		RenderTextBuffered(context);
	}
	else
	{
		TextView::OnRender(context);
	}
}

void Button::RecreateFrameBuffer(float width, float height)
{
	TextView::RecreateFrameBuffer(width, height);

	if (m_IsTextBuffered)
	{
		if (m_pFramebufferText)
		{
			if (GetWidth() == width && GetHeight() == height)
			{
				return;
			}
			delete m_pFramebufferText;
		}

		FramebufferDesc desc;
		desc.DepthStencilFormat = TEX_FORMAT_UNKNOWN;
		desc.ColorAttchmentFormats[0] = TEX_FORMAT_RGBA;
		desc.SamplingParams = { TEX_PARAM_EDGECLAMP, TEX_PARAM_LINEAR, TEX_PARAM_LINEAR };
		desc.NumColorAttachments = 1;
		desc.Width = static_cast<uint32>(width);
		desc.Height = static_cast<uint32>(height);

		m_pFramebufferText = new Framebuffer(desc);
	}
}
