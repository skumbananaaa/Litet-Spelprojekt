#include "..\..\Include\Scenes\SceneScenario.h"
#include "..\..\Include\Scenes\SceneGame.h"
#include "..\..\Include\Scenes\SceneMenu.h"
#include "../../Include/Game.h"

SceneScenario::SceneScenario()
{
	Game* game = Game::GetGame();
	Window* window = &game->GetWindow();

	m_pPanel = new Panel((window->GetWidth() - 600) / 2, (window->GetHeight() - 600) / 2, 600, 600);
	m_pButtonBack = new Button(0, m_pPanel->GetHeight() - 50, 100, 50, "Tillbaka");
	m_pButtonStart = new Button(m_pPanel->GetWidth() - m_pButtonBack->GetWidth(), m_pPanel->GetHeight() - 50, 100, 50, "Nästa");
	m_pTextViewTitle = new TextView(0, m_pPanel->GetHeight() - 50, m_pPanel->GetWidth(), 50, "Välj Scenarion", true);

	m_pButtonBack->SetBackgroundColor(GUIContext::COLOR_TRANSPARENT);
	m_pButtonStart->SetBackgroundColor(GUIContext::COLOR_TRANSPARENT);

	m_pButtonBack->AddButtonListener(this);
	m_pButtonStart->AddButtonListener(this);

	m_pPanel->Add(m_pButtonBack);
	m_pPanel->Add(m_pButtonStart);
	m_pPanel->Add(m_pTextViewTitle);

	game->GetGUIManager().Add(m_pPanel);
}

SceneScenario::~SceneScenario()
{
	Game::GetGame()->GetGUIManager().DeleteChildren();
}

void SceneScenario::CreateScenario(IScenario* scenario, int32 id) noexcept
{
	Button* button = new Button(0, 0, m_pListScrollable->GetWidth() - 20, 100, scenario->GetName());
	button->SetMargin(10, 5, 10, 5);
	button->SetSelected(true);
	button->SetUserData(reinterpret_cast<void*>(id));
	m_pListScrollable->Add(button);
	m_SelectionHandler.AddSelectable(button);
}

void SceneScenario::OnSelected(const SelectionHandler* handler, ISelectable* selection)
{

}

void SceneScenario::OnDeselected(const SelectionHandler* handler, ISelectable* selection)
{

}

void SceneScenario::OnButtonPressed(Button* button)
{

}

void SceneScenario::OnButtonReleased(Button* button)
{
	if (button == m_pButtonBack)
	{
		Game::GetGame()->SetScene(new SceneMenu());
	}
	else if (button == m_pButtonStart)
	{
		m_pPanel->SetVisible(false);
		std::vector<ISelectable*> selection;
		std::vector<int32> ids;
		m_SelectionHandler.GetSelection(selection);
		for (int i = 0; i < selection.size(); i++)
		{
			Button* button = (Button*)selection[i];
			ids.push_back(reinterpret_cast<int32>(button->GetUserData()));
		}
		ScenarioManager::SetEnabledScenarios(ids);
		Game::GetGame()->SetScene(new SceneGame());
	}
}

void SceneScenario::OnButtonHovered(Button* button)
{
}

void SceneScenario::OnButtonNotHovered(Button* button)
{
}