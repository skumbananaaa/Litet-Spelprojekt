#include "..\..\Include\Scenes\SceneCredits.h"
#include "..\..\Include\Scenes\SceneMenu.h"
#include "../../Include/Game.h"

SceneCredits::SceneCredits()
{
	
}

SceneCredits::~SceneCredits()
{
	Game::GetGame()->GetGUIManager().DeleteChildren();
}

void SceneCredits::OnActivated(SceneInternal* lastScene, IRenderer* m_pRenderer) noexcept
{
	SceneInternal::OnActivated(lastScene, m_pRenderer);

	Game* game = Game::GetGame();
	Window* window = &game->GetWindow();

	m_pPanel = new Panel((window->GetWidth() - 600) / 2, (window->GetHeight() - 600) / 2, 600, 600);
	m_pButtonBack = new Button(0, m_pPanel->GetHeight() - 50, 100, 50, "Tillbaka");
	m_pTextViewTitle = new TextView(0, m_pPanel->GetHeight() - 50, m_pPanel->GetWidth(), 50, "Utvecklare", true);

	m_pTextViewChristoffer = new TextView(0, m_pTextViewTitle->GetY() - 150, m_pPanel->GetWidth(), m_pButtonBack->GetHeight(), "Christoffer Andersson", true);
	m_pTextViewHerman = new TextView(0, m_pTextViewChristoffer->GetY() - 50, m_pPanel->GetWidth(), m_pButtonBack->GetHeight(), "Herman Hansson Söderlund", true);
	m_pTextViewAlexander = new TextView(0, m_pTextViewHerman->GetY() - 50, m_pPanel->GetWidth(), m_pButtonBack->GetHeight(), "Alexander Dahlin", true);
	m_pTextViewTim = new TextView(0, m_pTextViewAlexander->GetY() - 50, m_pPanel->GetWidth(), m_pButtonBack->GetHeight(), "Tim Mellander", true);
	m_pTextViewTimKort = new TextView(0, m_pTextViewTim->GetY() - 50, m_pPanel->GetWidth(), m_pButtonBack->GetHeight(), "Tim Johansson", true);
	m_pTextViewLeo = new TextView(0, m_pTextViewTimKort->GetY() - 50, m_pPanel->GetWidth(), m_pButtonBack->GetHeight(), "Leo Wikström", true);

	m_pButtonBack->SetBackgroundColor(GUIContext::COLOR_TRANSPARENT);
	m_pButtonBack->AddButtonListener(this);

	m_pPanel->SetBorderColor(GUIContext::COLOR_BLACK);
	m_pPanel->SetBoderThickness(3);

	m_pPanel->Add(m_pButtonBack);
	m_pPanel->Add(m_pTextViewTitle);
	m_pPanel->Add(m_pTextViewChristoffer);
	m_pPanel->Add(m_pTextViewHerman);
	m_pPanel->Add(m_pTextViewAlexander);
	m_pPanel->Add(m_pTextViewTim);
	m_pPanel->Add(m_pTextViewTimKort);
	m_pPanel->Add(m_pTextViewLeo);

	game->GetGUIManager().Add(m_pPanel);
}

void SceneCredits::OnDeactivated(SceneInternal* newScene) noexcept
{
	SceneInternal::OnDeactivated(newScene);

	Game* game = Game::GetGame();
	game->GetGUIManager().DeleteChildren();
}

void SceneCredits::OnUpdate(float dtS) noexcept
{
	SceneInternal::OnUpdate(dtS);
}

void SceneCredits::OnRender(float dtS) noexcept
{
	SceneInternal::OnRender(dtS);
}

void SceneCredits::OnButtonPressed(Button* button)
{
}

void SceneCredits::OnButtonReleased(Button* button)
{
	Game::GetGame()->SetScene(Game::GetGame()->m_pSceneMenu);
}

void SceneCredits::OnButtonHovered(Button* button)
{
}

void SceneCredits::OnButtonNotHovered(Button* button)
{
}