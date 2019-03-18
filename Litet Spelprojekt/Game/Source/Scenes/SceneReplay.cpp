#include "..\..\Include\Scenes\SceneReplay.h"
#include "../../Include/ReplayHandler.h"
#include "../../Include/Orders/OrderSchedule.h"
#include "../../Include/Scenarios/ScenarioManager.h"
#include <World/LightManager.h>
#include "../../Include/Game.h"
#include "../../Include/GameState.h"

SceneReplay::SceneReplay(World* pWorld) : SceneGame(pWorld, false)
{

}

SceneReplay::~SceneReplay()
{

}

void SceneReplay::OnActivated(SceneInternal * lastScene, IRenderer * m_pRenderer) noexcept
{
	SceneInternal::OnActivated(lastScene, m_pRenderer);

	CreateAudio();
	CreateGameObjects();
	CreateCrew();

	ScenarioManager::Init(m_pWorld);
	LightManager::Init(this, NUM_SPOT_LIGHTS);

	Game* game = Game::GetGame();
	Window* window = &game->GetWindow();

	m_pUICrewMember = new UICrewMember((window->GetWidth() - 330) / 2, window->GetHeight() - 170, 330, 170);
	m_pUILog = new UILog(window->GetWidth() - 600, window->GetHeight() - 700, 600, 700);

	game->GetGUIManager().Add(m_pUICrewMember);
	game->GetGUIManager().Add(m_pUILog);

	Logger::SetListener(m_pUILog);

	m_pUINotification = new UINotification(window->GetWidth() - 560, window->GetHeight(), 500, 50, 8.0F);

	SetPaused(false);

	OrderSchedule::Init(this);
	GameState::Reset();
}