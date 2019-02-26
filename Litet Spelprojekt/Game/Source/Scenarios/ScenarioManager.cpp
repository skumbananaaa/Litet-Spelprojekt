#include "..\..\Include\Scenarios\ScenarioManager.h"
#include <System/Random.h>
#include <World/Logger.h>
#include "..\..\Include\Game.h"

std::vector<IScenario*> ScenarioManager::s_Scenarios;
std::vector<int32> ScenarioManager::s_ActiveScenarios;
std::vector<int32> ScenarioManager::s_NonActiveScenarios;

uint32 ScenarioManager::RegisterScenario(IScenario* scenario) noexcept
{
	int32 id = s_Scenarios.size();
	s_Scenarios.push_back(scenario);
	//Game::GetGame()->GetUIScenario()->CreateScenario(scenario, id);
	return id;
}

void ScenarioManager::Release() noexcept
{
	for (IScenario* scenario : s_Scenarios)
	{
		Delete(scenario);
	}
}

void ScenarioManager::OnVisibilityChange(World* pWorld, Scene* pScene, const std::vector<uint32>& activeRooms) noexcept
{
	for (int i = s_ActiveScenarios.size() - 1; i >= 0; i--)
	{
		s_Scenarios[s_ActiveScenarios[i]]->OnVisibilityChange(pWorld, pScene, activeRooms);
	}
}

void ScenarioManager::Update(float dtS, World* world, Scene* scene, const std::vector<uint32>& activeRooms) noexcept
{
	for (int i = s_NonActiveScenarios.size() - 1; i >= 0; i--)
	{
		IScenario* scenario = s_Scenarios[s_NonActiveScenarios[i]];
		float time = scenario->GetTimeOfNextOutBreak() - dtS;
		if (time <= 0)
		{
			Logger::LogEvent("Scenario [" + scenario->GetName() + "] Started!");
			scenario->OnStart(scene);
			s_ActiveScenarios.push_back(s_NonActiveScenarios[i]);
			s_NonActiveScenarios.erase(s_NonActiveScenarios.begin() + i);
		}
		else
		{
			scenario->SetTimeOfNextOutBreak(time);
		}
	}

	for (int i = s_ActiveScenarios.size() - 1; i >= 0; i--)
	{
		IScenario* scenario = s_Scenarios[s_ActiveScenarios[i]];
		if (scenario->Update(dtS, world, scene, activeRooms))
		{
			Logger::LogEvent("Scenario [" + scenario->GetName() + "] Ended!");
			scenario->OnEnd();
			SetAsNonActive(s_ActiveScenarios[i]);
			s_ActiveScenarios.erase(s_ActiveScenarios.begin() + i);
		}
	}
}

void ScenarioManager::SetEnabledScenarios(const std::vector<int32> ids) noexcept
{
	s_NonActiveScenarios = ids;
	s_ActiveScenarios.clear();
}

void ScenarioManager::SetAsNonActive(int id)
{
	IScenario* scenario = s_Scenarios[id];
	s_NonActiveScenarios.push_back(id);
	Random::GenerateInt(scenario->GetCooldownTime(), scenario->GetMaxTimeBeforeOutbreak());
}