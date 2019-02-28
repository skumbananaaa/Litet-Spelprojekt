#include "..\..\Include\Scenarios\ScenarioMissile.h"
#include <System/Random.h>
#include <World/Logger.h>

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
	glm::vec3 pos = glm::vec3(Random::GenerateInt(-50, 50), 200, Random::GenerateInt(-50, 50));
	glm::vec3 target = glm::vec3(Random::GenerateInt(1, 10), 0, Random::GenerateInt(1, 40));
	m_pGameObjectMissile = new GameObjectMissile(pos, target);

	scene->AddGameObject(m_pGameObjectMissile);
}

void ScenarioMissile::OnEnd(Scene* scene) noexcept
{
	scene->RemoveGameObject(m_pGameObjectMissile);
	DeleteSafe(m_pGameObjectMissile);
}

void ScenarioMissile::OnVisibilityChange(World* pWorld, Scene* pScene, const std::vector<uint32>& activeRooms) noexcept
{

}

bool ScenarioMissile::Update(float dtS, World* world, Scene* scene, const std::vector<uint32>& activeRooms) noexcept
{
	if (m_pGameObjectMissile)
	{
		if (m_pGameObjectMissile->HasHitTarget())
		{
			return true;
		}
		return false;
	}
	return true;
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