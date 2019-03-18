#include "..\..\Include\Scenarios\ScenarioManager.h"
#include "..\..\Include\Game.h"
#include <System/Random.h>
#include <World/Logger.h>

std::vector<IScenario*> ScenarioManager::s_Scenarios;
std::vector<int32> ScenarioManager::s_ActiveScenarios;
std::vector<int32> ScenarioManager::s_NonActiveScenarios;

uint32 ScenarioManager::RegisterScenario(IScenario* scenario) noexcept
{
	int32 id = s_Scenarios.size();
	s_Scenarios.push_back(scenario);
	return id;
}

void ScenarioManager::Release() noexcept
{
	for (IScenario* scenario : s_Scenarios)
	{
		Delete(scenario);
	}
}

void ScenarioManager::OnVisibilityChange(World* pWorld, SceneGame* pScene) noexcept
{
	for (int i = s_ActiveScenarios.size() - 1; i >= 0; i--)
	{
		s_Scenarios[s_ActiveScenarios[i]]->OnVisibilityChange(pWorld, pScene);
	}
}

void ScenarioManager::Update(float dtS, World* world, SceneGame* scene) noexcept
{
	for (int i = s_NonActiveScenarios.size() - 1; i >= 0; i--)
	{
		IScenario* scenario = s_Scenarios[s_NonActiveScenarios[i]];
		float time = scenario->GetTimeOfNextOutBreak() - dtS;
		if (time <= 0)
		{
			StartScenario(s_NonActiveScenarios[i]);
		}
		else
		{
			scenario->SetTimeOfNextOutBreak(time);
		}
	}

	for (int i = s_ActiveScenarios.size() - 1; i >= 0; i--)
	{
		IScenario* scenario = s_Scenarios[s_ActiveScenarios[i]];
		if (scenario->Update(dtS, world, scene))
		{
			Logger::LogEvent("Scenario [" + scenario->GetName() + "] avslutades!");
			scenario->OnEnd(scene);
			SetAsNonActive(s_ActiveScenarios[i]);
			s_ActiveScenarios.erase(s_ActiveScenarios.begin() + i);
		}
	}
}

void ScenarioManager::SetEnabledScenarios(const std::vector<int32>& ids) noexcept
{
	s_ActiveScenarios.clear();
	s_NonActiveScenarios.clear();
	for (int i = ids.size() - 1; i >= 0; i--)
	{
		SetAsNonActive(ids[i]);
	}
}

bool ScenarioManager::StartScenario(int32 index) noexcept
{
	for (int i = s_NonActiveScenarios.size() - 1; i >= 0; i--)
	{
		if (index == s_NonActiveScenarios[i])
		{
			Logger::LogEvent("Scenario [" + s_Scenarios[index]->GetName() + "] startade!");
			s_Scenarios[index]->OnStart(Game::GetGame()->m_pSceneGame);
			s_ActiveScenarios.push_back(index);
			s_NonActiveScenarios.erase(s_NonActiveScenarios.begin() + i);
			return true;
		}
	}
	return false;
}

void ScenarioManager::Escalate(int32 index, const glm::ivec3& position) noexcept
{
	if (StartScenario(index))
	{
		s_Scenarios[index]->Escalate(position);
	}
	else
	{
		for (int i = s_ActiveScenarios.size() - 1; i >= 0; i--)
		{
			if (index == s_ActiveScenarios[i])
			{
				s_Scenarios[index]->Escalate(position);
				return;
			}
		}
	}
}

void ScenarioManager::Init(World* pWorld)
{
	for (IScenario* scenario : s_Scenarios)
	{
		scenario->Init(pWorld);
		scenario->SetTimeOfNextOutBreak(Random::GenerateInt(scenario->GetCooldownTime(), scenario->GetCooldownTime() + scenario->GetMaxTimeBeforeOutbreak()));
	}
}

void ScenarioManager::Reset() noexcept
{
	for (IScenario* scenario : s_Scenarios)
	{
		scenario->Release();
		scenario->SetTimeOfNextOutBreak(Random::GenerateInt(scenario->GetCooldownTime(), scenario->GetCooldownTime() + scenario->GetMaxTimeBeforeOutbreak()));
	}


	for (int i = 0; i < s_ActiveScenarios.size(); i++)
	{
		s_NonActiveScenarios.push_back(s_ActiveScenarios[i]);
	}

	s_ActiveScenarios.clear();
}

const std::vector<IScenario*>& ScenarioManager::GetScenarios() noexcept
{
	return s_Scenarios;
}

void ScenarioManager::SetAsNonActive(int id)
{
	IScenario* scenario = s_Scenarios[id];
	s_NonActiveScenarios.push_back(id);
	scenario->SetTimeOfNextOutBreak(Random::GenerateInt(scenario->GetCooldownTime(), scenario->GetCooldownTime() + scenario->GetMaxTimeBeforeOutbreak()));
}