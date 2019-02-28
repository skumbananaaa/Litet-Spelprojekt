#include "..\..\Include\GUI\UILog.h"

UILog::UILog(float x, float y, float width, float height) : Panel(x, y, width, height)
{
	m_pTextViewLog = new TextView(0, height - 50, width, 50, "Loggbok", true);
	m_ListScrollableLog = new ListScrollable(0, 0, width, height - m_pTextViewLog->GetHeight());
	m_ListScrollableLog->SetBackgroundColor(glm::vec4(0.15F, 0.15F, 0.15F, 1.0F));
	Add(m_pTextViewLog);
	Add(m_ListScrollableLog);
}

UILog::~UILog()
{

}

void UILog::OnLogged(const std::string& text) noexcept
{
	glm::vec4 color = m_ListScrollableLog->GetNrOfChildren() % 2 == 0 ? glm::vec4(0.2F, 0.2F, 0.2F, 1.0F) : glm::vec4(0.3F, 0.3F, 0.3F, 1.0F);
	TextView* textView = new TextView(0, 0, m_ListScrollableLog->GetClientWidth(), 40, text);
	textView->SetBackgroundColor(color);
	m_ListScrollableLog->Add(textView);
}

void UILog::PrintName() const
{
	std::cout << "UILog" << std::endl;
}