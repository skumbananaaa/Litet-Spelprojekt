#include "..\..\Include\Scenes\SceneMenu.h"
#include "..\..\Include\Scenes\SceneOptions.h"
#include "..\..\Include\Scenes\SceneScenario.h"
#include "..\..\Include\Scenes\SceneCredits.h"
#include "../../Include/Game.h"

SceneMenu::SceneMenu()
{
	Game* game = Game::GetGame();
	Window* window = &game->GetWindow();

	m_pPanel = new Panel((window->GetWidth() - 600) / 2, (window->GetHeight() - 600) / 2, 600, 600);
	m_pButtonPlay = new Button(100, m_pPanel->GetHeight() - 125, m_pPanel->GetWidth() - 200, 100, "Spela");
	m_pButtonOptions = new Button(100, m_pButtonPlay->GetY() - 125, m_pPanel->GetWidth() - 200, 100, "Alternativ");
	m_pButtonCredits = new Button(100, m_pButtonOptions->GetY() - 125, m_pPanel->GetWidth() - 200, 100, "Utvecklare");
	m_pButtonQuit = new Button(100, m_pButtonCredits->GetY() - 125, m_pPanel->GetWidth() - 200, 100, "Avsluta");

	m_pButtonPlay->AddButtonListener(this);
	m_pButtonOptions->AddButtonListener(this);
	m_pButtonCredits->AddButtonListener(this);
	m_pButtonQuit->AddButtonListener(this);

	m_pPanel->Add(m_pButtonPlay);
	m_pPanel->Add(m_pButtonOptions);
	m_pPanel->Add(m_pButtonCredits);
	m_pPanel->Add(m_pButtonQuit);

	game->GetGUIManager().Add(m_pPanel);
}

SceneMenu::~SceneMenu()
{
	Game* game = Game::GetGame();
	game->GetGUIManager().DeleteChildren();
}

void SceneMenu::OnUpdate(float dtS) noexcept
{
	SceneInternal::OnUpdate(dtS);
}

void SceneMenu::OnRender(float dtS) noexcept
{
	SceneInternal::OnRender(dtS);
}

void SceneMenu::OnButtonPressed(Button* button)
{
}

void SceneMenu::OnButtonReleased(Button* button)
{
	if (button == m_pButtonPlay)
	{
		Game::GetGame()->SetScene(new SceneScenario());
	}
	else if (button == m_pButtonOptions)
	{
		Game::GetGame()->SetScene(new SceneOptions());
	}
	else if (button == m_pButtonCredits)
	{
		Game::GetGame()->SetScene(new SceneCredits());
	}
	else if (button == m_pButtonQuit)
	{
		Game::GetGame()->Exit();
	}
}

void SceneMenu::OnButtonHovered(Button* button)
{
}

void SceneMenu::OnButtonNotHovered(Button* button)
{
}
