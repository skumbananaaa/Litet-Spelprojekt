#include "..\Include\ScenarioFire.h"

ScenarioFire::ScenarioFire()
{
}

void ScenarioFire::OnStart() noexcept
{
	
}

void ScenarioFire::OnEnd() noexcept
{
	
}

bool ScenarioFire::Update(float dtS, World* world, Scene* scene) noexcept
{
	return true;
}

std::string ScenarioFire::GetName() noexcept
{
	return "Fire";
}

int32 ScenarioFire::GetCooldownTime() noexcept
{
	return 20;
}

int32 ScenarioFire::GetMaxTimeBeforeOutbreak() noexcept
{
	return 60;
}