#include "..\..\Include\Scenes\SceneLoading.h"
#include "../../Include/Game.h"
#include "../../Include/Scenes/SceneMenu.h"

SceneLoading::SceneLoading() :
	m_pTextViewFile(nullptr),
	m_pLoadingBar(nullptr)
{
	
}

SceneLoading::~SceneLoading()
{
	
}

void SceneLoading::OnActivated(SceneInternal* lastScene, IRenderer* pRenderer) noexcept
{
	SceneInternal::OnActivated(lastScene, pRenderer);

	Game* game = Game::GetGame();
	Window* window = &game->GetWindow();

	m_pTextViewFile = new TextView((window->GetWidth() - 300.0f) / 2.0f, (window->GetHeight() - 50.0f) / 2.0f + 50.0f, 300.0f, 50.0f, "Laddar...");
	m_pLoadingBar = new ProgressBar((window->GetWidth() - 300.0f) / 2.0f, (window->GetHeight() - 50.0f) / 2.0f, 300.0f, 50.0f);

	game->GetGUIManager().Add(m_pTextViewFile);
	game->GetGUIManager().Add(m_pLoadingBar);
}

void SceneLoading::OnUpdate(float dtS) noexcept
{
	SceneInternal::OnUpdate(dtS);
}

void SceneLoading::OnRender(float dtS) noexcept
{
	SceneInternal::OnRender(dtS);
}

void SceneLoading::OnResourceLoading(const std::string& file, float percentage)
{
	m_pTextViewFile->SetText("Laddar: " + file);
	m_pLoadingBar->SetPercentage(percentage);
}

void SceneLoading::OnResourcesLoaded()
{
	Game::GetGame()->SetScene(Game::GetGame()->m_pSceneMenu);
}