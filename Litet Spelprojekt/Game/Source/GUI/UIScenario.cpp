#include "..\..\Include\GUI\UIScenario.h"
#include "..\..\Include\Scenarios\ScenarioManager.h"
#include "..\..\Include\Game.h"

UIScenario::UIScenario(float x, float y, float width, float height) : Panel(x, y, width, height),
	m_SelectionHandler(false, true)
{
	m_pButtonBack = new Button(0, height - 50, 100, 50, "Back");
	m_pButtonStart = new Button(width - m_pButtonBack->GetWidth(), height - 50, 100, 50, "Play");
	m_pTextViewTitle = new TextView(0, m_pButtonBack->GetY(), width, m_pButtonBack->GetHeight(), "Select Scenarios", true);
	m_pListScrollable = new ListScrollable(0, 0, width, height - m_pButtonBack->GetHeight());

	m_pButtonBack->SetBackgroundColor(GUIContext::COLOR_TRANSPARENT);
	m_pButtonStart->SetBackgroundColor(GUIContext::COLOR_TRANSPARENT);

	m_pButtonBack->AddButtonListener(this);
	m_pButtonStart->AddButtonListener(this);

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

void UIScenario::CreateScenario(IScenario* scenario, int32 id) noexcept
{
	Button* button = new Button(0, 0, m_pListScrollable->GetWidth() - 20, 100, scenario->GetName());
	button->SetMargin(10, 5, 10, 5);
	button->SetSelected(true);
	button->SetUserData(reinterpret_cast<void*>(id));
	m_pListScrollable->Add(button);
	m_SelectionHandler.AddSelectable(button);
}

void UIScenario::OnSelected(const SelectionHandler* handler, ISelectable* selection)
{

}

void UIScenario::OnDeselected(const SelectionHandler* handler, ISelectable* selection)
{

}

void UIScenario::OnButtonPressed(Button* button)
{

}

void UIScenario::OnButtonReleased(Button* button)
{
	if (button == m_pButtonBack)
	{

	}
	else if (button == m_pButtonStart)
	{
		this->SetVisible(false);
		std::vector<ISelectable*> selection;
		std::vector<int32> ids;
		m_SelectionHandler.GetSelection(selection);
		for (int i = 0; i < selection.size(); i++)
		{
			Button* button = (Button*)selection[i];
			ids.push_back(reinterpret_cast<int32>(button->GetUserData()));
		}
		ScenarioManager::SetEnabledScenarios(ids);
		Game::GetGame()->StartGame();
	}
}

void UIScenario::OnButtonHovered(Button* button)
{
}

void UIScenario::OnButtonNotHovered(Button* button)
{
}

void UIScenario::PrintName() const
{
	std::cout << "UIScenario" << std::endl;
}

