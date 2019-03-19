#include "..\..\Include\Scenes\SceneInstructions.h"
#include "../../Include/Game.h"
#include "../../Include/Orders/OrderSchedule.h"
#include "../../Include/Scenarios/ScenarioManager.h"

SceneInstructions::SceneInstructions() : 
	m_pPanel(nullptr),
	m_pSceneGame(nullptr),
	m_HasRenderedUI(false)
{
}

void SceneInstructions::OnActivated(SceneInternal* lastScene, IRenderer* m_pRenderer) noexcept
{
	SceneInternal::OnActivated(lastScene, m_pRenderer);

	Game* game = Game::GetGame();
	Window* window = &game->GetWindow();

	m_pPanel = new Panel(0.0f, 0.0f, window->GetWidth(), window->GetHeight());
	m_pPanel->SetBackgroundTexture(ResourceHandler::GetTexture2D(TEXTURE::LOADING_CONTROLS));
	m_pPanel->AddExternalRenderer(this);
	m_HasRenderedUI = false;

	m_pButtonContinue = new Button(m_pPanel->GetWidth() - 311, 95, 200, 100, "Laddar...");

	m_pButtonContinue->AddButtonListener(this);
	m_pButtonContinue->SetTextColor(GUIContext::COLOR_BLACK);

	m_pPanel->Add(m_pButtonContinue);

	game->GetGUIManager().Add(m_pPanel);
}

void SceneInstructions::CreateWorld() noexcept
{
	ResourceHandler::ResetGameObjectCounters();
	World* pWorld = WorldSerializer::Read("world.json");

	////Enable clipplane for wallmaterial
	ResourceHandler::GetMaterial(MATERIAL::WALL_STANDARD)->SetCullMode(CULL_MODE_NONE);
	ResourceHandler::GetMaterial(MATERIAL::BULKHEADS_STANDARD)->SetCullMode(CULL_MODE_NONE);

	//SetClipPlanes(0);

	m_pSceneGame = new SceneGame(pWorld, true);

	// Generate rooms
	pWorld->Generate(*m_pSceneGame);

	pWorld->GenerateRoomShadows(*m_pSceneGame);

	m_pButtonContinue->SetText("Fortsätt");
	m_pButtonContinue->SetBackgroundColor(m_pButtonContinue->GetOnHoverColor());
}

void SceneInstructions::OnUpdate(float dtS) noexcept
{
	SceneInternal::OnUpdate(dtS);

	if (m_HasRenderedUI)
	{
		if (!m_pSceneGame)
		{
			CreateWorld();
		}
	}
}

void SceneInstructions::OnRender(float dtS) noexcept
{
	SceneInternal::OnRender(dtS);
}

void SceneInstructions::OnButtonPressed(Button* button)
{
}

void SceneInstructions::OnButtonReleased(Button* button)
{
	if (m_pSceneGame)
	{
		OrderSchedule::Release();
		ScenarioManager::Reset();
		Game::GetGame()->StartGame(m_pSceneGame);
		m_pSceneGame = nullptr;
	}
}

void SceneInstructions::OnButtonHovered(Button* button)
{
}

void SceneInstructions::OnButtonNotHovered(Button* button)
{
}

void SceneInstructions::OnRenderGUIObject(GUIContext* context, GUIObject* object)
{
	m_HasRenderedUI = true;
}

SceneInstructions::~SceneInstructions()
{

}