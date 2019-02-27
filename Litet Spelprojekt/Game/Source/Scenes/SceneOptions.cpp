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
}

void SceneOptions::OnDeactivated(SceneInternal* newScene) noexcept
{
	SceneInternal::OnDeactivated(newScene);
}

void SceneOptions::OnUpdate(float dtS) noexcept
{
	SceneInternal::OnUpdate(dtS);
}

void SceneOptions::OnRender(float dtS) noexcept
{
	SceneInternal::OnRender(dtS);
}
