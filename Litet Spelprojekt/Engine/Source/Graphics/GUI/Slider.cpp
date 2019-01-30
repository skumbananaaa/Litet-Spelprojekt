#include <EnginePch.h>
#include <Graphics\GUI\Slider.h>

Slider::Slider(float x, float y, float width, float height, Texture2D* textureBackground, Texture2D* textureForeground, void(*onChangedCallback)(Slider*, float)) : GUIObject(x, y, width, height),
	m_pTextureForeground(textureForeground),
	m_IsPressed(false),
	m_MouseOffset(0),
	m_SliderPos(0),
	m_Ratio(1)
{
	SetTexture(textureBackground);
	m_OnChangedCallback = onChangedCallback;
}

Slider::~Slider()
{

}

bool Slider::isVertical() const noexcept
{
	return GetWidth() < GetHeight();
}

void Slider::SetForgroundTexture(Texture2D* texture)
{
	if (m_pTextureForeground != texture)
	{
		m_pTextureForeground = texture;
		RequestRepaint();
	}
}

Texture2D* Slider::GetForegroundTexture() const
{
	return m_pTextureForeground;
}

void Slider::OnAdded(GUIObject* parent)
{
	AddRealTimeRenderer(this);
	AddMouseListener(this);
}

void Slider::OnRemoved(GUIObject* parent)
{
	RemoveRealTimeRenderer(this);
	RemoveMouseListener(this);
}

void Slider::OnMousePressed(const glm::vec2& position, MouseButton mousebutton)
{
	if (ContainsPoint(position))
	{
		m_IsPressed = true;
		if (isVertical())
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

void Slider::OnMouseMove(const glm::vec2& lastPosition, const glm::vec2& position)
{
	if (m_IsPressed)
	{
		static float lastValue = 0;
		lastValue = GetPercentage();

		if (isVertical())
		{
			m_SliderPos = position.y - m_MouseOffset;
			if (m_SliderPos < 0)
			{
				m_SliderPos = 0;
			}
			else if (GetPercentage() < 0)
			{
				SetPercentage(0);
			}
		}
		else
		{
			m_SliderPos = position.x - m_MouseOffset;
			if (m_SliderPos < 0)
			{
				m_SliderPos = 0;
			}
			else if (GetPercentage() > 1)
			{
				SetPercentage(1);
			}		
		}

		static float currentValue = 0;
		currentValue = GetPercentage();
		if (lastValue != currentValue)
		{
			if (m_OnChangedCallback)
			{
				m_OnChangedCallback(this, currentValue);
			}
		}
	}
}

void Slider::RenderRealTime(GUIContext* context)
{
	if (m_pTextureForeground)
	{
		float x = GetXInWorld();
		float y = GetYInWorld();

		if (isVertical())
		{
			float indent = GetWidth() * 0.1;
			float width = GetWidth() - indent * 2;
			float height = GetHeight() * m_Ratio - indent * 2;

			context->SetVertexQuadData(x + indent, y + m_SliderPos + indent, width, height);
		}
		else
		{
			float indent = GetHeight() * 0.1;
			float width = GetWidth() * m_Ratio - indent * 2;
			float height = GetHeight() - indent * 2;
			
			context->SetVertexQuadData(x + m_SliderPos + indent, y + indent, width, height);
		}

		context->GetGraphicsContext()->SetTexture(m_pTextureForeground, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		context->GetGraphicsContext()->SetTexture(nullptr, 0);
	}
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
	if (isVertical())
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
	if (m_Ratio == 0)
	{
		return 0;
	}

	if (isVertical())
	{
		return 1.0 - m_SliderPos / (GetHeight() - GetHeight() * m_Ratio);
	}
	else
	{
		return m_SliderPos / (GetWidth() - GetWidth() * m_Ratio);
	}
}

void Slider::SetOnSliderChanged(void(*callback)(Slider*, float))
{
	m_OnChangedCallback = callback;
}