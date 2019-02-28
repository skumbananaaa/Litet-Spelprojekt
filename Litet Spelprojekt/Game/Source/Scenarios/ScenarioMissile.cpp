#include "..\..\Include\Scenarios\ScenarioMissile.h"

ScenarioMissile::ScenarioMissile()
{

}

ScenarioMissile::~ScenarioMissile()
{

}

void ScenarioMissile::Init(World* pWorld) noexcept
{

}

void ScenarioMissile::OnStart(Scene* scene) noexcept
{

}

void ScenarioMissile::OnEnd() noexcept
{

}

void ScenarioMissile::OnVisibilityChange(World* pWorld, Scene* pScene, const std::vector<uint32>& activeRooms) noexcept
{

}

bool ScenarioMissile::Update(float dtS, World* world, Scene* scene, const std::vector<uint32>& activeRooms) noexcept
{
	return false;
}

std::string ScenarioMissile::GetName() noexcept
{
	return "Missilträff";
}

int32 ScenarioMissile::GetCooldownTime() noexcept
{
	return 0;
}

int32 ScenarioMissile::GetMaxTimeBeforeOutbreak() noexcept
{
	return 1;
}