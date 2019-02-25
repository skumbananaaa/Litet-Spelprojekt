#include "..\..\Include\GUI\UIScenario.h"

UIScenario::UIScenario(float x, float y, float width, float height) : Panel(x, y, width, height),
	m_SelectionHandler(false, true)
{
	m_pButtonBack = new Button(0, height - 50, 100, 50, "Back");
	m_pButtonStart = new Button(width - m_pButtonBack->GetWidth(), height - 50, 100, 50, "Play");
	m_pTextViewTitle = new TextView(0, m_pButtonBack->GetY(), width, m_pButtonBack->GetHeight(), "Select Scenarios", true);
	m_pListScrollable = new ListScrollable(0, 0, width, height - m_pButtonBack->GetHeight());

	m_pButtonBack->SetBackgroundColor(GUIContext::COLOR_TRANSPARENT);
	m_pButtonStart->SetBackgroundColor(GUIContext::COLOR_TRANSPARENT);

	m_SelectionHandler.AddSelectionListener(this);

	Add(m_pButtonBack);
	Add(m_pButtonStart);
	Add(m_pTextViewTitle);
	Add(m_pListScrollable);

	SetDeleteAllChildrenOnDestruction(true);
}

UIScenario::~UIScenario()
{

}

void UIScenario::CreateScenario(IScenario* scenario) noexcept
{
	Button* button = new Button(0, 0, m_pListScrollable->GetWidth() - 20, 100, scenario->GetName());
	button->SetMargin(10, 5, 10, 5);
	button->SetSelected(true);
	m_pListScrollable->Add(button);
	m_SelectionHandler.AddSelectable(button);
}

void UIScenario::OnSelected(const SelectionHandler* handler, ISelectable* selection)
{

}

void UIScenario::OnDeselected(const SelectionHandler* handler, ISelectable* selection)
{

}

void UIScenario::PrintName() const
{
	std::cout << "UIScenario" << std::endl;
}

