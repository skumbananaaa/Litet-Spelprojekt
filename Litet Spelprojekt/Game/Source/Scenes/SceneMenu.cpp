#include "..\..\Include\Scenes\SceneMenu.h"
#include "..\..\Include\Scenes\SceneOptions.h"
#include "..\..\Include\Scenes\SceneScenario.h"
#include "..\..\Include\Scenes\SceneCredits.h"
#include "../../Include/Game.h"
#include <Audio/Sources/AudioSource.h>
#include "../../Include/ReplayHandler.h"

SceneMenu::SceneMenu()
{
	m_pAudioSourceSelect = AudioSource::CreateSoundSource(SOUND::UI_SELECT);
	m_pAudioSourceSelect->SetRollOffFactor(10.0f);
	m_pAudioSourceSelect->SetReferenceDistance(0.0f);
	m_pAudioSourceSelect->SetMaxDistance(500.0f);
	m_pAudioSourceSelect->SetLooping(false);

	m_pAudioSourceHover = AudioSource::CreateSoundSource(SOUND::UI_HOVER);
	m_pAudioSourceHover->SetRollOffFactor(10.0f);
	m_pAudioSourceHover->SetReferenceDistance(0.0f);
	m_pAudioSourceHover->SetMaxDistance(500.0f);
	m_pAudioSourceHover->SetLooping(false);
}

SceneMenu::~SceneMenu()
{
	DeleteSafe(m_pAudioSourceSelect);
	DeleteSafe(m_pAudioSourceHover);
}

void SceneMenu::OnActivated(SceneInternal* lastScene, IRenderer* pRenderer) noexcept
{
	SceneInternal::OnActivated(lastScene, pRenderer);

	Game* game = Game::GetGame();
	Window* window = &game->GetWindow();

	ReplayHandler::Reset();

	m_pPanel = new Panel((window->GetWidth() - 600) / 2, (window->GetHeight() - 600) / 2, 600, 600);
	m_pButtonPlay = new Button(100, m_pPanel->GetHeight() - 160, m_pPanel->GetWidth() - 200, 100, "Spela");
	m_pButtonOptions = new Button(100, m_pButtonPlay->GetY() - 125, m_pPanel->GetWidth() - 200, 100, "Alternativ");
	m_pButtonCredits = new Button(100, m_pButtonOptions->GetY() - 125, m_pPanel->GetWidth() - 200, 100, "Utvecklare");
	m_pButtonQuit = new Button(100, m_pButtonCredits->GetY() - 125, m_pPanel->GetWidth() - 200, 100, "Avsluta");

	m_pPanel->SetBackgroundColor(GUIContext::COLOR_TRANSPARENT);

	m_pButtonPlay->SetBackgroundColor(GUIContext::COLOR_PANEL_BACKGROUND);
	m_pButtonOptions->SetBackgroundColor(GUIContext::COLOR_PANEL_BACKGROUND);
	m_pButtonCredits->SetBackgroundColor(GUIContext::COLOR_PANEL_BACKGROUND);
	m_pButtonQuit->SetBackgroundColor(GUIContext::COLOR_PANEL_BACKGROUND);

	m_pButtonPlay->SetOnHoverColor(GUIContext::COLOR_PANEL_MIDGROUND);
	m_pButtonOptions->SetOnHoverColor(GUIContext::COLOR_PANEL_MIDGROUND);
	m_pButtonCredits->SetOnHoverColor(GUIContext::COLOR_PANEL_MIDGROUND);
	m_pButtonQuit->SetOnHoverColor(GUIContext::COLOR_PANEL_MIDGROUND);

	m_pButtonPlay->SetBorderColor(GUIContext::COLOR_BLACK);
	m_pButtonOptions->SetBorderColor(GUIContext::COLOR_BLACK);
	m_pButtonCredits->SetBorderColor(GUIContext::COLOR_BLACK);
	m_pButtonQuit->SetBorderColor(GUIContext::COLOR_BLACK);

	m_pButtonPlay->SetBoderThickness(3);
	m_pButtonOptions->SetBoderThickness(3);
	m_pButtonCredits->SetBoderThickness(3);
	m_pButtonQuit->SetBoderThickness(3);

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
	m_pAudioSourceSelect->Play();
}

void SceneMenu::OnButtonReleased(Button* button)
{
	if (button == m_pButtonPlay)
	{
		Game::GetGame()->SetScene(Game::GetGame()->m_pSceneScenario);
	}
	else if (button == m_pButtonOptions)
	{
		Game::GetGame()->SetScene(Game::GetGame()->m_pSceneOptions);
	}
	else if (button == m_pButtonCredits)
	{
		Game::GetGame()->SetScene(Game::GetGame()->m_pSceneCredits);
	}
	else if (button == m_pButtonQuit)
	{
		Game::GetGame()->Exit();
	}
}

void SceneMenu::OnButtonHovered(Button* button)
{
	m_pAudioSourceHover->Stop();
	m_pAudioSourceHover->Play();
}

void SceneMenu::OnButtonNotHovered(Button* button)
{
}