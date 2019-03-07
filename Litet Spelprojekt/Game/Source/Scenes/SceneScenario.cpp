#include "..\..\Include\Scenes\SceneScenario.h"
#include "..\..\Include\Scenes\SceneGame.h"
#include "..\..\Include\Scenes\SceneMenu.h"
#include "../../Include/Game.h"
#include "../../Include/Scenarios/ScenarioManager.h"
#include <Audio/Sources/AudioSource.h>

SceneScenario::SceneScenario() : 
	m_SelectionHandler(false, true)
{
	m_pAudioSourceSelect = AudioSource::CreateSoundSource(SOUND::UI_SELECT);
	m_pAudioSourceSelect->SetRollOffFactor(10.0f);
	m_pAudioSourceSelect->SetReferenceDistance(0.0f);
	m_pAudioSourceSelect->SetMaxDistance(500.0f);
	m_pAudioSourceSelect->SetLooping(false);

	m_pAudioSourceDeselect = AudioSource::CreateSoundSource(SOUND::UI_DESELECT);
	m_pAudioSourceDeselect->SetRollOffFactor(10.0f);
	m_pAudioSourceDeselect->SetReferenceDistance(0.0f);
	m_pAudioSourceDeselect->SetMaxDistance(500.0f);
	m_pAudioSourceDeselect->SetLooping(false);

	m_pAudioSourceHover = AudioSource::CreateSoundSource(SOUND::UI_HOVER);
	m_pAudioSourceHover->SetRollOffFactor(10.0f);
	m_pAudioSourceHover->SetReferenceDistance(0.0f);
	m_pAudioSourceHover->SetMaxDistance(500.0f);
	m_pAudioSourceHover->SetLooping(false);
}

SceneScenario::~SceneScenario()
{
	DeleteSafe(m_pAudioSourceSelect);
	DeleteSafe(m_pAudioSourceDeselect);
	DeleteSafe(m_pAudioSourceHover);
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
		button->AddExternalRenderer(this);
		m_pListScrollable->Add(button);
		m_SelectionHandler.AddSelectable(button);
	}

	game->GetGUIManager().Add(m_pPanel);

	m_SelectionHandler.AddSelectionListener(this);
}

void SceneScenario::OnDeactivated(SceneInternal* newScene) noexcept
{
	m_SelectionHandler.Release();

	SceneInternal::OnDeactivated(newScene);
}

void SceneScenario::OnSelected(const SelectionHandler* handler, ISelectable* selection)
{
	m_pAudioSourceSelect->Play();
}

void SceneScenario::OnDeselected(const SelectionHandler* handler, ISelectable* selection)
{
	m_pAudioSourceDeselect->Play();
}

void SceneScenario::OnButtonPressed(Button* button)
{
	m_pAudioSourceSelect->Play();
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
		Game::GetGame()->StartGame();
	}
}

void SceneScenario::OnButtonHovered(Button* button)
{
	m_pAudioSourceHover->Play();
}

void SceneScenario::OnButtonNotHovered(Button* button)
{
}

void SceneScenario::OnRenderGUIObject(GUIContext* context, GUIObject* object)
{
	Button* button = (Button*)object;

	if (button->IsSelected())
	{
		context->RenderTexture(ResourceHandler::GetTexture2D(TEXTURE::ICON_CHECKED), object->GetWidth() - 70, 20, 50, 50, GUIContext::COLOR_WHITE);
	}
	else
	{
		context->RenderTexture(ResourceHandler::GetTexture2D(TEXTURE::ICON_CROSSED), object->GetWidth() - 70, 20, 50, 50, GUIContext::COLOR_WHITE);
	}
}
