#include "../../Include/GUI/UIOrder.h"
#include "../../Include/Game.h"

Panel* UIOrder::m_pPanel = nullptr;

UIOrder::UIOrder()
{
	
}

UIOrder::~UIOrder()
{
}

void UIOrder::AddChoice(const std::string& name, void* userData) noexcept
{
	m_Choices.push_back(std::pair<std::string, void*>(name, userData));
}

void UIOrder::OnButtonPressed(Button * button)
{
}

void UIOrder::OnButtonReleased(Button * button)
{
	OnOrderChosen(button->GetText(), button->GetUserData(), m_SelectedMembers);
	m_pPanel->SetVisible(false);
}

void UIOrder::OnButtonHovered(Button * button)
{
}

void UIOrder::OnButtonNotHovered(Button * button)
{
}

void UIOrder::DisplayOrders(int32 x, int32 y, const std::vector<int32>& selectedMembers)
{
	DisplayOrders(x, y, this);
	m_Choices.clear();
	m_SelectedMembers = selectedMembers;
}

void UIOrder::DisplayOrders(int32 x, int32 y, UIOrder* uiOrder)
{
	if (m_pPanel)
	{
		Game::GetGame()->GetGUIManager().Remove(m_pPanel);
		m_pPanel = nullptr;
	}

	m_pPanel = new Panel(x, y, 200, uiOrder->m_Choices.size() * 30);
	for (int i = 0; i < uiOrder->m_Choices.size(); i++)
	{
		Button* button = new Button(0, i * 30, m_pPanel->GetWidth(), 30, uiOrder->m_Choices[i].first);
		button->SetUserData(uiOrder->m_Choices[i].second);
		button->SetTextCentered(false);
		button->AddButtonListener(uiOrder);
		m_pPanel->Add(button);
	}
	Game::GetGame()->GetGUIManager().Add(m_pPanel);
}