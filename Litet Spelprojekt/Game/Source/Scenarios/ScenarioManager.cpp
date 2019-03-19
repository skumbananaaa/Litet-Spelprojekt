#include "..\..\Include\Scenarios\ScenarioManager.h"
#include "..\..\Include\Game.h"
#include "../../Include/GameState.h"
#include <System/Random.h>
#include <World/Logger.h>
#include "../../Include/ReplayHandler.h"

std::vector<IScenario*> ScenarioManager::s_Scenarios;
std::vector<int32> ScenarioManager::s_ActiveScenarios;
std::vector<int32> ScenarioManager::s_NonActiveScenarios;

uint32 ScenarioManager::RegisterScenario(IScenario* scenario) noexcept
{
	int32 id = (int32)s_Scenarios.size();
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
	for (int i = (int32)s_ActiveScenarios.size() - 1; i >= 0; i--)
	{
		s_Scenarios[s_ActiveScenarios[i]]->OnVisibilityChange(pWorld, pScene);
	}
}

void ScenarioManager::Update(float dtS, World* world, SceneGame* scene) noexcept
{
	if (!ReplayHandler::IsReplaying())
	{
		for (int i = (int32)s_NonActiveScenarios.size() - 1; i >= 0; i--)
		{
			IScenario* scenario = s_Scenarios[s_NonActiveScenarios[i]];
			float time = scenario->GetTimeOfNextOutBreak() - dtS;
			if (time <= 0)
			{
				if (!scenario->IsComplete())
    			{
    				StartScenario(s_NonActiveScenarios[i]);
    			}
			}
			else
			{
				scenario->SetTimeOfNextOutBreak(time);
			}
		}
	}

	for (int i = (int32)s_ActiveScenarios.size() - 1; i >= 0; i--)
	{
		IScenario* scenario = s_Scenarios[s_ActiveScenarios[i]];
		if (scenario->Update(dtS, world, scene))
		{
			Logger::LogEvent("Scenario [" + scenario->GetName() + "] avslutades!");
			scenario->OnEnd(scene);
			SetAsNonActive(s_ActiveScenarios[i]);
			s_ActiveScenarios.erase(s_ActiveScenarios.begin() + i);

			if (scenario->IsComplete())
			{
				std::cout << "Scenario '" << scenario->GetName() << "' is complete" << std::endl;
				GameState::AddScenariosCompleted();
			}
		}
	}
}

void ScenarioManager::SetEnabledScenarios(const std::vector<int32>& ids) noexcept
{
	s_ActiveScenarios.clear();
	s_NonActiveScenarios.clear();
	for (int i = (int32)ids.size() - 1; i >= 0; i--)
	{
		SetAsNonActive(ids[i]);
	}
	GameState::SetNumScenariosSelected((uint32)s_NonActiveScenarios.size());
}

bool ScenarioManager::StartScenario(int32 index) noexcept
{
	for (int i = (int32)s_NonActiveScenarios.size() - 1; i >= 0; i--)
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

bool ScenarioManager::StartScenario(IScenario* pScenario) noexcept
{
	for (int i = (int32)s_NonActiveScenarios.size() - 1; i >= 0; i--)
	{
		if (pScenario == s_Scenarios[s_NonActiveScenarios[i]])
		{
			Logger::LogEvent("Scenario [" + pScenario->GetName() + "] startade!");
			pScenario->OnStart(Game::GetGame()->m_pSceneGame);
			s_ActiveScenarios.push_back(s_NonActiveScenarios[i]);
			s_NonActiveScenarios.erase(s_NonActiveScenarios.begin() + i);
			return true;
		}
	}
	return false;
}

void ScenarioManager::Escalate(int32 index, const glm::ivec3& position, float severity) noexcept
{
	if (!ReplayHandler::IsReplaying())
	{
		if (StartScenario(index))
		{
			s_Scenarios[index]->Escalate(position, severity);
		}
		else
		{
			for (int i = (int32)s_ActiveScenarios.size() - 1; i >= 0; i--)
			{
				if (index == s_ActiveScenarios[i])
				{
					s_Scenarios[index]->Escalate(position, severity);
					return;
				}
			}
		}
	}
}

void ScenarioManager::Init(World* pWorld)
{
	for (IScenario* scenario : s_Scenarios)
	{
		scenario->Init(pWorld);
		scenario->SetTimeOfNextOutBreak((float)Random::GenerateInt(10, scenario->GetMaxTimeBeforeOutbreak()));
	}
}

void ScenarioManager::Reset() noexcept
{
	for (IScenario* scenario : s_Scenarios)
	{
		scenario->Release();
		scenario->SetTimeOfNextOutBreak((float)Random::GenerateInt(10, scenario->GetMaxTimeBeforeOutbreak()));
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
	scenario->SetTimeOfNextOutBreak((float)Random::GenerateInt(scenario->GetCooldownTime(), scenario->GetCooldownTime() + scenario->GetMaxTimeBeforeOutbreak()));
}