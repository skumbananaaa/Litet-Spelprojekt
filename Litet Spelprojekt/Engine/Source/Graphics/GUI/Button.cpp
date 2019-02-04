#include <EnginePch.h>
#include <Graphics\GUI\Button.h>

Button::Button(float x, float y, float width, float height, const std::string& text, void(*onPressedCallback)(Button*), void(*onReleasedCallback)(Button*), int textSize) : TextView(x, y, width, height, text, TextAlignment::CENTER, textSize),
	m_pOnPressedTexture(nullptr),
	m_IsPressed(false),
	m_IsHovered(false),
	m_PressedColor(0.8F, 0.8F, 0.8F, 1.0F),
	m_HoverColor(0.6F, 0.6F, 0.6F, 1.0F),
	m_SelectedColor(0.553F, 0.824F, 0.541F, 1.0F),
	m_PressedTextColor(0.0F, 0.0F, 0.0F, 1.0F),
	m_HoverTextColor(1.0F, 1.0F, 1.0F, 1.0F),
	m_SelectedTextColor(0.0F, 0.0F, 0.0F, 1.0F),
	m_OnPressedCallback(onPressedCallback),
	m_OnReleasedCallback(onReleasedCallback)
{
	SetTextAlignment(CENTER);
	SetBackgroundColor(glm::vec4(0.408F, 0.408F, 0.408F, 1.0F));
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
		for (IButtonListener* listener : m_ButtonListeners)
		{
			listener->OnButtonPressed(this);
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
		for (IButtonListener* listener : m_ButtonListeners)
		{
			listener->OnButtonReleased(this);
		}
		for (ISelectableListener* listener : GetSelectionListeners())
		{
			listener->OnSelected(this);
		}
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
		}
	}
	else if (m_IsHovered)
	{
		m_IsHovered = false;
		RequestRepaint();
	}
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
