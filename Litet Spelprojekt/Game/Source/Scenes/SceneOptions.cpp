#include "..\..\Include\Scenes\SceneOptions.h"
#include "../../Include/Game.h"

SceneOptions::SceneOptions()
{
}

SceneOptions::~SceneOptions()
{
}

void SceneOptions::OnUpdate(float dtS) noexcept
{
	SceneInternal::OnUpdate(dtS);
}

void SceneOptions::OnRender(float dtS) noexcept
{
	SceneInternal::OnRender(dtS);
}
