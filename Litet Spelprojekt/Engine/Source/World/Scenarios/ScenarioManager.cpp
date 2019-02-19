#include <EnginePch.h>
#include <World/Scenarios/ScenarioManager.h>
#include <System/Random.h>
#include <World/Logger.h>

std::vector<IScenario*> ScenarioManager::m_Scenarios;
std::vector<int32> ScenarioManager::m_ActiveSenarios;
std::vector<int32> ScenarioManager::m_NonActiveSenarios;

uint32 ScenarioManager::RegisterScenario(IScenario* scenario) noexcept
{
	m_Scenarios.push_back(scenario);
	SetAsNonActive(m_Scenarios.size() - 1);
	return m_Scenarios.size() - 1;
}

void ScenarioManager::Release() noexcept
{
	for (IScenario* scenario : m_Scenarios)
	{
		Delete(scenario);
	}
}

void ScenarioManager::Update(float dtS, World* world, Scene* scene, const std::vector<uint32>& activeRooms) noexcept
{
	for (int i = m_NonActiveSenarios.size() - 1; i >= 0; i--)
	{
		IScenario* scenario = m_Scenarios[m_NonActiveSenarios[i]];
		float time = scenario->GetTimeOfNextOutBreak() - dtS;
		if (time <= 0)
		{
			Logger::LogEvent("Scenario [" + scenario->GetName() + "] Started!");
			scenario->OnStart();
			m_ActiveSenarios.push_back(m_NonActiveSenarios[i]);
			m_NonActiveSenarios.erase(m_NonActiveSenarios.begin() + i);
		}
		else
		{
			scenario->SetTimeOfNextOutBreak(time);
		}
	}

	for (int i = m_ActiveSenarios.size() - 1; i >= 0; i--)
	{
		IScenario* scenario = m_Scenarios[m_ActiveSenarios[i]];
		if (scenario->Update(dtS, world, scene, activeRooms))
		{
			Logger::LogEvent("Scenario [" + scenario->GetName() + "] Ended!");
			scenario->OnEnd();
			SetAsNonActive(m_ActiveSenarios[i]);
			m_ActiveSenarios.erase(m_ActiveSenarios.begin() + i);
		}
	}
}

void ScenarioManager::SetAsNonActive(int id)
{
	IScenario* scenario = m_Scenarios[id];
	m_NonActiveSenarios.push_back(id);
	scenario->SetTimeOfNextOutBreak(Random::GenerateInt(scenario->GetCooldownTime(), scenario->GetMaxTimeBeforeOutbreak()));
}