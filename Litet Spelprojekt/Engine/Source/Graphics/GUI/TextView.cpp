#include <EnginePch.h>
#include <Graphics/GUI/TextView.h>

TextView::TextView(float x, float y, float width, float height, const std::string& text, int size) : GUIObject(x, y, width, height), 
	m_Text(text), 
	m_Size(size), 
	m_TextAlignment(CENTER_VERTICAL)
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

void TextView::SetSize(int size)
{
	if (m_Size != size)
	{
		m_Size = size;
		RequestRepaint();
	}
}

int TextView::GetSize() const
{
	return m_Size;
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

void TextView::OnRender(GLContext* context, FontRenderer* fontRenderer)
{
	if (!m_Text.empty())
	{
		glm::vec2 size = fontRenderer->CalculateSize(m_Text, 0.4);
		glm::vec2 position(0, 0);

		switch (m_TextAlignment)
		{
		case CENTER:
			position.x = (GetWidth() - size.x) / 2;
			position.y = (GetHeight() - size.y) / 2;
			break;
		case CENTER_VERTICAL:
			position.y = (GetHeight() - size.y) / 2;
			break;
		case CENTER_HORIZONTAL:
			position.x = (GetWidth() - size.x) / 2;
			break;
		default:
			break;
		}

		fontRenderer->RenderText(context, m_Text, position.x, position.y, m_Size / 100.0F);
	}
}