#include <EnginePch.h>
#include <Graphics/GUI/TextView.h>

TextView::TextView(float x, float y, float width, float height, const std::string& text, bool center, int textSize) : GUIObject(x, y, width, height),
	m_Text(text), 
	m_TextSize(textSize),
	m_TextCentered(center),
	m_TextColor(1.0F, 1.0F, 1.0F, 1.0F)
{
	SetBackgroundColor(glm::vec4(1.0F, 1.0F, 1.0F, 0.0F));
}

TextView::~TextView()
{

}

void TextView::SetText(const std::string& text)
{
	if (m_Text.compare(text) != 0)
	{
		m_Text = text;
		RequestRepaint();
	}
}

const std::string& TextView::GetText() const
{
	return m_Text;
}

void TextView::SetTextSize(int size)
{
	if (m_TextSize != size)
	{
		m_TextSize = size;
		RequestRepaint();
	}
}

int TextView::GetTextSize() const
{
	return m_TextSize;
}

void TextView::SetTextCentered(bool centered)
{
	if (m_TextCentered != centered)
	{
		m_TextCentered = centered;
		RequestRepaint();
	}
}

bool TextView::IsTextCentered()
{
	return m_TextCentered;
}

const glm::vec4& TextView::GetTextColor() const noexcept
{
	return m_TextColor;
}

void TextView::SetTextColor(const glm::vec4& color)
{
	if (m_TextColor != color)
	{
		m_TextColor = color;
		RequestRepaint();
	}
}

void TextView::OnRender(GUIContext* context)
{
	GUIObject::OnRender(context);
	RenderText(context);
}

void TextView::RenderText(GUIContext* context)
{
	if (!m_Text.empty())
	{
		float scale = m_TextSize / 100.0f;

		if (m_TextCentered)
		{
			context->RenderText(m_Text, GetWidth() / 2, GetHeight() / 2, GetWidth(), GetHeight(), scale, GetClearTextColor(), CENTER);
		}
		else
		{
			context->RenderText(m_Text, 0, GetHeight() / 2, GetWidth(), GetHeight(), scale, GetClearTextColor(), CENTER_VERTICAL);
		}
	}
}

void TextView::PrintName() const
{
	std::cout << "TextView";
}

const glm::vec4& TextView::GetClearTextColor() const
{
	return GetTextColor();
}
