#include "..\..\Include\Scenes\SceneReplay.h"
#include "../../Include/ReplayHandler.h"
#include "../../Include/Orders/OrderSchedule.h"
#include "../../Include/Scenarios/ScenarioManager.h"
#include <World/LightManager.h>
#include "../../Include/Game.h"
#include "../../Include/GameState.h"

SceneReplay::SceneReplay(World* pWorld, float percentage) : SceneGame(pWorld, false)
{
	m_BeginPercentage = percentage;
}

SceneReplay::~SceneReplay()
{

}

void SceneReplay::OnActivated(SceneInternal * lastScene, IRenderer* pRenderer) noexcept
{
	SceneInternal::OnActivated(lastScene, pRenderer);

	CreateAudio();
	CreateGameObjects();
	CreateCrew();

	ScenarioManager::Init(m_pWorld);
	LightManager::Init(this, NUM_SPOT_LIGHTS);

	Game* game = Game::GetGame();
	Window* window = &game->GetWindow();

	m_pUICrewMember = new UICrewMember((window->GetWidth() - 330.0f) / 2.0f, window->GetHeight() - 170.0f, 330.0f, 170.0f);
	m_pUILog = new UILog(window->GetWidth() - 600.0f, window->GetHeight() - 700.0f, 600.0f, 700.0f);
	m_UIReplay = new UIReplay(0.0f, 0.0f, (float)window->GetWidth(), 30.0f, m_BeginPercentage);

	game->GetGUIManager().Add(m_pUICrewMember);
	game->GetGUIManager().Add(m_pUILog);
	game->GetGUIManager().Add(m_UIReplay);

	Logger::SetListener(m_pUILog);

	m_pUINotification = new UINotification(window->GetWidth() - 560.0f, (float)window->GetHeight(), 500.0f, 50.0f, 8.0F);

	SetPaused(false);

	OrderSchedule::Init(this);
	GameState::Reset();
}

void SceneReplay::OnUpdate(float dtS) noexcept
{
	SceneGame::OnUpdate(dtS);
	if (ReplayHandler::GetElapsedTime() >= ReplayHandler::GetTotalTime())
	{
		m_IsGameOver = true;
	}
}
