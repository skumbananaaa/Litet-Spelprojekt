#include "..\..\Include\Scenes\SceneScenario.h"
#include "..\..\Include\Scenes\SceneGame.h"
#include "..\..\Include\Scenes\SceneMenu.h"
#include "../../Include/Game.h"
#include "../../Include/Scenarios/ScenarioManager.h"

SceneScenario::SceneScenario() : 
	m_SelectionHandler(false, true)
{
	
}

SceneScenario::~SceneScenario()
{
	Game::GetGame()->GetGUIManager().DeleteChildren();
}

void SceneScenario::OnActivated(SceneInternal* lastScene, IRenderer* m_pRenderer) noexcept
{
	SceneInternal::OnActivated(lastScene, m_pRenderer);

	Game* game = Game::GetGame();
	Window* window = &game->GetWindow();

	m_pPanel = new Panel((window->GetWidth() - 600) / 2, (window->GetHeight() - 600) / 2, 600, 600);
	m_pButtonBack = new Button(0, m_pPanel->GetHeight() - 50, 100, 50, "Tillbaka");
	m_pButtonStart = new Button(m_pPanel->GetWidth() - m_pButtonBack->GetWidth(), m_pPanel->GetHeight() - 50, 100, 50, "Nästa");
	m_pTextViewTitle = new TextView(0, m_pPanel->GetHeight() - 50, m_pPanel->GetWidth(), 50, "Välj Scenarion", true);
	m_pListScrollable = new ListScrollable(0, 0, m_pPanel->GetWidth(), m_pPanel->GetHeight() - m_pButtonBack->GetHeight());

	m_pButtonBack->SetBackgroundColor(GUIContext::COLOR_TRANSPARENT);
	m_pButtonStart->SetBackgroundColor(GUIContext::COLOR_TRANSPARENT);

	m_pButtonBack->AddButtonListener(this);
	m_pButtonStart->AddButtonListener(this);

	m_pPanel->SetBorderColor(GUIContext::COLOR_BLACK);
	m_pPanel->SetBoderThickness(3);

	m_pListScrollable->SetBackgroundColor(GUIContext::COLOR_TRANSPARENT);

	m_pPanel->Add(m_pButtonBack);
	m_pPanel->Add(m_pButtonStart);
	m_pPanel->Add(m_pTextViewTitle);
	m_pPanel->Add(m_pListScrollable);

	const std::vector<IScenario*>& scenarios = ScenarioManager::GetScenarios();
	for (int i = 0; i < scenarios.size(); i++)
	{
		Button* button = new Button(0, 0, m_pListScrollable->GetWidth() - 20, 100, scenarios[i]->GetName());
		button->SetMargin(10, 5, 10, 5);
		button->SetSelected(true);
		button->SetUserData(reinterpret_cast<void*>(i));
		m_pListScrollable->Add(button);
		m_SelectionHandler.AddSelectable(button);
	}

	game->GetGUIManager().Add(m_pPanel);
}

void SceneScenario::OnDeactivated(SceneInternal* newScene) noexcept
{
	SceneInternal::OnDeactivated(newScene);

	m_SelectionHandler.Release();

	Game* game = Game::GetGame();
	game->GetGUIManager().DeleteChildren();
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
		Game::GetGame()->SetScene(Game::GetGame()->m_pSceneMenu);
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
		Game::GetGame()->SetScene(Game::GetGame()->m_pSceneGame);
	}
}

void SceneScenario::OnButtonHovered(Button* button)
{
}

void SceneScenario::OnButtonNotHovered(Button* button)
{
}