#include "..\..\Include\Scenes\SceneLoading.h"
#include "../../Include/Game.h"
#include "../../Include/Scenes/SceneMenu.h"

SceneLoading::SceneLoading()
{
	Game* game = Game::GetGame();
	Window* window = &game->GetWindow();

	m_pTextViewFile = new TextView((window->GetWidth() - 300) / 2, (window->GetHeight() - 50) / 2 + 50, 300, 50, "Loading...");
	m_pLoadingBar = new ProgressBar((window->GetWidth() - 300) / 2, (window->GetHeight() - 50) / 2, 300, 50);

	game->GetGUIManager().Add(m_pTextViewFile);
	game->GetGUIManager().Add(m_pLoadingBar);
}

SceneLoading::~SceneLoading()
{
	Game* game = Game::GetGame();
	game->GetGUIManager().DeleteChildren();
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
	m_pTextViewFile->SetText("Loading: " + file);
	m_pLoadingBar->SetPercentage(percentage);
}

void SceneLoading::OnResourcesLoaded()
{
	Game::GetGame()->SetScene(new SceneMenu());
}