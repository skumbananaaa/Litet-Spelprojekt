#include <EnginePch.h>
#include <Graphics/GUI/TextView.h>

TextView::TextView(float x, float y, float width, float height, const std::string& text, int textSize) : GUIObject(x, y, width, height),
	m_Text(text), 
	m_TextSize(textSize),
	m_TextAlignment(CENTER_VERTICAL)
{

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

void TextView::SetTextAlignment(TextAlignment textAlignment)
{
	if (m_TextAlignment != textAlignment)
	{
		m_TextAlignment = textAlignment;
		RequestRepaint();
	}
}

TextAlignment TextView::GetTextAlignment()
{
	return m_TextAlignment;
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
		context->GetFontRenderer()->UpdateRenderTargetSize(GetWidth(), GetHeight());

		float scale = m_TextSize / 100.0f;
		glm::vec2 size = context->CalculateTextSize(m_Text, scale);
		float x = 0;
		float y = 0;

		switch (m_TextAlignment)
		{
		case CENTER:
			x = (GetWidth() - size.x) / 2;
			y = (GetHeight() - size.y) / 2;
			break;
		case CENTER_VERTICAL:
			y = (GetHeight() - size.y) / 2;
			break;
		case CENTER_HORIZONTAL:
			x = (GetWidth() - size.x) / 2;
			break;
		default:
			break;
		}

		context->RenderText(m_Text, x, y, scale);
	}
}

void TextView::PrintName() const
{
	std::cout << "TextView";
}
