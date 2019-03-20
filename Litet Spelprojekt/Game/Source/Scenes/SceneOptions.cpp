#include "..\..\Include\Scenes\SceneOptions.h"
#include "../../Include/Game.h"
#include <Audio/Sources/AudioSource.h>

SceneOptions::SceneOptions()
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

SceneOptions::~SceneOptions()
{
	DeleteSafe(m_pAudioSourceSelect);
	DeleteSafe(m_pAudioSourceHover);
}

void SceneOptions::OnActivated(SceneInternal* lastScene, IRenderer* pRenderer) noexcept
{
	SceneInternal::OnActivated(lastScene, pRenderer);

	Game* game = Game::GetGame();
	Window* window = &game->GetWindow();

	m_pPanel = new Panel((window->GetWidth() - 600.0f) / 2.0f, (window->GetHeight() - 600.0f) / 2.0f, 600.0f, 600.0f);
	m_pButtonBack = new Button(0, m_pPanel->GetHeight() - 50.0f, 100.0f, 50.0f, "Tillbaka");
	m_pTextViewTitle = new TextView(0, m_pPanel->GetHeight() - 50.0f, m_pPanel->GetWidth(), 50.0f, "Alternativ", true);

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
	m_pAudioSourceSelect->Play();
}

void SceneOptions::OnButtonReleased(Button* button)
{
	Game::GetGame()->SetScene(Game::GetGame()->m_pSceneMenu);
}

void SceneOptions::OnButtonHovered(Button* button)
{
	m_pAudioSourceHover->Play();
}

void SceneOptions::OnButtonNotHovered(Button* button)
{
}