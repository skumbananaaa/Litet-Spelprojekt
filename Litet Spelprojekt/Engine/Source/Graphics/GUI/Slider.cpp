#include <EnginePch.h>
#include <Graphics\GUI\Slider.h>

Slider::Slider(float x, float y, float width, float height, Texture2D* textureBackground, Texture2D* textureForeground) : GUIObject(x, y, width, height),
	m_pTextureForeground(textureForeground),
	m_IsPressed(false),
	m_Offset(0),
	m_Ratio(1),
	m_Percentage(0)
{
	SetTexture(textureBackground);
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
		//AddRealTimeRenderer(this);
		if (isVertical())
		{
			
		}
		else
		{
			m_Offset = position.x;
		}
	}
}

void Slider::OnMouseReleased(const glm::vec2& position, MouseButton mousebutton)
{
	if (m_IsPressed)
	{
		//RemoveRealTimeRenderer(this);
		m_IsPressed = false;
	}
}

void Slider::OnMouseMove(const glm::vec2& lastPosition, const glm::vec2& position)
{
	if (m_IsPressed)
	{
		if (isVertical())
		{

		}
		else
		{
			float off =  position.x - m_Offset;
			m_Percentage = off / (GetWidth() * m_Ratio);
			std::cout << m_Percentage << std::endl;
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

		}
		else
		{
			float indent = GetHeight() * 0.1;
			float width = GetWidth() * m_Ratio - indent * 2;
			float height = GetHeight() - indent * 2;

			float xOffset = (GetWidth() - GetWidth() * m_Ratio) * m_Percentage;

			context->SetVertexQuadData(x + xOffset + indent, y + indent, width, height);
			context->GetGraphicsContext()->SetTexture(m_pTextureForeground, 0);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			context->GetGraphicsContext()->SetTexture(nullptr, 0);
		}
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
	m_Percentage = percentage;
}

float Slider::GetPercentage() const noexcept
{
	return m_Percentage;
}
