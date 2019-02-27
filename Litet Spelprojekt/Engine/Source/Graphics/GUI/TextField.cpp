#include <EnginePch.h>
#include <Graphics/GUI/TextField.h>

SelectionHandler TextField::s_SelectionHandler(false);

TextField::TextField(float x, float y, float width, float height, TextFieldType type, const std::string& text, int textSize) :
	Button(x, y, width, height, text, nullptr, nullptr, textSize)
{
	AddKeyboardListener(this);
	s_SelectionHandler.AddSelectable(this);
	SetTextCentered(false);
	m_Type = type;
}

TextField::~TextField()
{
	RemoveKeyboardListener(this);
	s_SelectionHandler.RemoveSelectable(this);
}

uint32 TextField::GetTextAsUInt()
{
	if (m_Type == NUMBERS_ONLY)
	{
		return std::stoul(GetText());
	}

	return 0;
}

void TextField::OnKeyUp(KEY keycode)
{
	if (IsSelected())
	{
	}
}

void TextField::OnKeyDown(KEY keycode)
{
	if (IsSelected())
	{
		switch (keycode)
		{
			case KEY_BACKSPACE:
			{
				const std::string& currentString = GetText();
				SetText(currentString.substr(0, currentString.size() - 1));
				break;
			}
			case KEY_DELETE:
			{
				SetText("");
				break;
			}
			default:
			{

			}
		}
	}
}

void TextField::OnCharFromKey(char c)
{
	if (IsSelected())
	{
		if (CanPrintChar(c))
		{
			SetText(GetText() + c);
		}
	}
}

bool TextField::CanPrintChar(char c)
{
	if (m_Type == ALL_CHARACTERS)
	{
		return true;
	}
	else if (m_Type == NUMBERS_ONLY)
	{
		if (c >= '0' && c <= '9')
		{
			return true;
		}
	}
	else if (m_Type == LETTERS_ONLY)
	{
		if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
		{
			return true;
		}
	}

	return false;
}
