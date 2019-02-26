#include <EnginePch.h>
#include <Graphics\GUI\Slider.h>

Slider::Slider(float x, float y, float width, float height, void(*onChangedCallback)(Slider*, float)) : GUIObject(x, y, width, height),
	m_IsPressed(false),
	m_IsHovered(false),
	m_MouseOffset(0),
	m_SliderPos(0),
	m_SliderVel(0),
	m_Ratio(1),
	m_SliderColor(GUIContext::COLOR_BUTTON_BACKGROUND),
	m_PressedColor(GUIContext::COLOR_PRESSED),
	m_HoverColor(GUIContext::COLOR_HOVER)
{
	m_OnChangedCallback = onChangedCallback;
	SetBackgroundColor(glm::vec4(0.243F, 0.243F, 0.259F, 1.0F));
}

Slider::~Slider()
{

}

bool Slider::IsVertical() const noexcept
{
	return GetWidth() < GetHeight();
}

void Slider::OnAdded(GUIObject* parent)
{
	AddRealTimeRenderer();
	AddMouseListener(this);
}

void Slider::OnRemoved(GUIObject* parent)
{
	RemoveRealTimeRenderer();
	RemoveMouseListener(this);
}

void Slider::OnMousePressed(const glm::vec2& position, MouseButton mousebutton)
{
	if (ContainsPoint(position))
	{
		m_IsPressed = true;
		if (IsVertical())
		{
			m_MouseOffset = position.y - m_SliderPos;
		}
		else
		{
			m_MouseOffset = position.x - m_SliderPos;
		}
	}
}

void Slider::OnMouseReleased(const glm::vec2& position, MouseButton mousebutton)
{
	if (m_IsPressed)
	{
		m_IsPressed = false;
	}
}

void Slider::OnMouseMove(const glm::vec2& position)
{
	if (m_IsPressed)
	{
		if (IsVertical())
		{
			MoveSlider(position.y - m_MouseOffset - m_SliderPos);
		}
		else
		{
			MoveSlider(position.x - m_MouseOffset - m_SliderPos);
		}
	}
	else
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
}

void Slider::OnMouseScroll(const glm::vec2& position, const glm::vec2& offset)
{
	if (ContainsPoint(position))
	{
		if (IsVertical())
		{
			AccelerateSlider(offset.y * 600 * GetRatio());
		}
		else
		{
			AccelerateSlider(-offset.y * 600 * GetRatio());
		}
	}
}

void Slider::RenderRealTime(GUIContext* context, float x, float y)
{
	float width = 0;
	float height = 0;

	if (IsVertical())
	{
		float indent = GetWidth() * 0.2;
		width = GetWidth() - indent * 2;
		height = GetHeight() * m_Ratio - indent * 2;
		x += indent;
		y += m_SliderPos + indent;
	}
	else
	{
		float indent = GetHeight() * 0.2;
		width = GetWidth() * m_Ratio - indent * 2;
		height = GetHeight() - indent * 2;
		x += m_SliderPos + indent;
		y += indent;
	}
	context->RenderTexture(GetDefaultTexture(), x, y, width, height, GetSliderClearColor());
}

void Slider::PrintName() const
{
	std::cout << "Slider";
}

void Slider::SetRatio(float ratio)
{
	m_Ratio = ratio;
}

float Slider::GetRatio() const noexcept
{
	return m_Ratio;
}

void Slider::SetPercentage(float percentage)
{
	if (IsVertical())
	{
		percentage = 1.0 - percentage;
		m_SliderPos = percentage * (GetHeight() - GetHeight() * m_Ratio);
	}
	else
	{
		m_SliderPos = percentage * (GetWidth() - GetWidth() * m_Ratio);
	}
}

float Slider::GetPercentage() const noexcept
{
	if (m_Ratio == 1)
	{
		return 0;
	}

	if (IsVertical())
	{
		return 1.0 - m_SliderPos / (GetHeight() - GetHeight() * m_Ratio);
	}
	else
	{
		return m_SliderPos / (GetWidth() - GetWidth() * m_Ratio);
	}
}

const glm::vec4& Slider::GetSliderColor() const noexcept
{
	return m_SliderColor;
}

void Slider::SetSliderColor(const glm::vec4& color)
{
	if (m_SliderColor != color)
	{
		m_SliderColor = color;
	}
}

const glm::vec4& Slider::GetOnPressedColor() const noexcept
{
	return m_PressedColor;
}

void Slider::SetOnPressedColor(const glm::vec4& color)
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

const glm::vec4& Slider::GetOnHoverColor() const noexcept
{
	return m_HoverColor;
}

void Slider::SetOnHoverColor(const glm::vec4& color)
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

void Slider::AddSliderListener(ISliderListener* listener)
{
	if (!Contains<ISliderListener>(m_SliderListeners, listener))
	{
		m_SliderListeners.push_back(listener);
	}
	else
	{
		std::cout << "SliderListener already added" << std::endl;
	}
}

void Slider::RemoveSliderListener(ISliderListener* listener)
{
	int32 counter = 0;
	for (ISliderListener* object : m_SliderListeners)
	{
		if (object == listener)
		{
			m_SliderListeners.erase(m_SliderListeners.begin() + counter);
			return;
		}
		counter++;
	}
}

void Slider::SetOnSliderChanged(void(*callback)(Slider*, float))
{
	m_OnChangedCallback = callback;
}

void Slider::MoveSlider(float offset)
{
	static float lastValue = 0;
	lastValue = GetPercentage();

	m_SliderPos += offset;

	if (GetPercentage() < 0)
	{
		SetPercentage(0);
		m_SliderVel = 0;
	}
	else if (GetPercentage() > 1)
	{
		SetPercentage(1);
		m_SliderVel = 0;
	}

	static float currentValue = 0;
	currentValue = GetPercentage();
	if (lastValue != currentValue)
	{
		if (m_OnChangedCallback)
		{
			m_OnChangedCallback(this, currentValue);
		}
		for (ISliderListener* listener : m_SliderListeners)
		{
			listener->OnSliderChange(this, currentValue);
		}
	}
}

void Slider::AccelerateSlider(float offset)
{
	m_SliderVel += offset;

	if (m_SliderVel > 2000 * GetRatio())
	{
		m_SliderVel = 2000 * GetRatio();
	}
	else if (m_SliderVel < -2000 * GetRatio())
	{
		m_SliderVel = -2000 * GetRatio();
	}
}

const glm::vec4& Slider::GetSliderClearColor() const
{
	if (m_IsPressed)
	{
		return GetOnPressedColor();
	}
	else if (m_IsHovered)
	{
		return GetOnHoverColor();
	}
	return GetSliderColor();
}

void Slider::OnUpdate(float dtS)
{
	MoveSlider(m_SliderVel * dtS);

	if (m_SliderVel > 0)
	{
		m_SliderVel -= 1500.0 * dtS * GetRatio();
		if (m_SliderVel < 0)
		{
			m_SliderVel = 0;
		}
	}
	else if(m_SliderVel < 0)
	{
		m_SliderVel += 1500.0 * dtS * GetRatio();
		if (m_SliderVel > 0)
		{
			m_SliderVel = 0;
		}
	}
}