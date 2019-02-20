#include "..\..\Include\Scenarios\ScenarioFire.h"

ScenarioFire::ScenarioFire()
{
}

void ScenarioFire::OnStart() noexcept
{
	
}

void ScenarioFire::OnEnd() noexcept
{
	
}

void ScenarioFire::OnVisibilityChange(World* pWorld, Scene* pScene, const std::vector<uint32>& activeRooms)
{
}

bool ScenarioFire::Update(float dtS, World* pWorld, Scene* pScene, const std::vector<uint32>& activeRooms) noexcept
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