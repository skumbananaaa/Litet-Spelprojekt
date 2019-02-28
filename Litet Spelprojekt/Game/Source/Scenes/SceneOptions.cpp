#include "..\..\Include\Scenes\SceneOptions.h"
#include "../../Include/Game.h"

SceneOptions::SceneOptions()
{
}

SceneOptions::~SceneOptions()
{
}

void SceneOptions::OnActivated(SceneInternal* lastScene, IRenderer* m_pRenderer) noexcept
{
	SceneInternal::OnActivated(lastScene, m_pRenderer);

	Game* game = Game::GetGame();
	Window* window = &game->GetWindow();

	m_pPanel = new Panel((window->GetWidth() - 600) / 2, (window->GetHeight() - 600) / 2, 600, 600);
	m_pButtonBack = new Button(0, m_pPanel->GetHeight() - 50, 100, 50, "Tillbaka");
	m_pTextViewTitle = new TextView(0, m_pPanel->GetHeight() - 50, m_pPanel->GetWidth(), 50, "Alternativ", true);

	m_pButtonBack->SetBackgroundColor(GUIContext::COLOR_TRANSPARENT);
	m_pButtonBack->AddButtonListener(this);

	m_pPanel->SetBorderColor(GUIContext::COLOR_BLACK);
	m_pPanel->SetBoderThickness(3);

	m_pPanel->Add(m_pButtonBack);
	m_pPanel->Add(m_pTextViewTitle);

	game->GetGUIManager().Add(m_pPanel);
}

void SceneOptions::OnUpdate(float dtS) noexcept
{
	SceneInternal::OnUpdate(dtS);
}

void SceneOptions::OnRender(float dtS) noexcept
{
	SceneInternal::OnRender(dtS);
}

void SceneOptions::OnButtonPressed(Button* button)
{
}

void SceneOptions::OnButtonReleased(Button* button)
{
	Game::GetGame()->SetScene(Game::GetGame()->m_pSceneMenu);
}

void SceneOptions::OnButtonHovered(Button* button)
{
}

void SceneOptions::OnButtonNotHovered(Button* button)
{
}