#include <EnginePch.h>
#include <World\ScenarioManager.h>
#include <System/Random.h>
#include <World/Logger.h>

std::vector<IScenario*> ScenarioManager::m_Scenarios;
std::vector<int32> ScenarioManager::m_ActiveScenarios;
std::vector<int32> ScenarioManager::m_NonActiveScenarios;

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

void ScenarioManager::Update(float dtS, World* world, Scene* scene) noexcept
{
	for (int i = m_NonActiveScenarios.size() - 1; i >= 0; i--)
	{
		IScenario* scenario = m_Scenarios[m_NonActiveScenarios[i]];
		float time = scenario->GetTimeOfNextOutBreak() - dtS;

		if (glm::floor(time) != glm::floor(scenario->GetTimeOfNextOutBreak()))
		{
			if (time >= 0)
			{
				std::cout << glm::floor(time) + 1 << std::endl;
			}
			else
			{
				std::cout << "FIRE\n";
			}
		}

		if (time <= 0)
		{
			Logger::LogEvent("Scenario [" + scenario->GetName() + "] Started!");
			scenario->OnStart();
			m_ActiveScenarios.push_back(m_NonActiveScenarios[i]);
			m_NonActiveScenarios.erase(m_NonActiveScenarios.begin() + i);
		}
		else
		{
			scenario->SetTimeOfNextOutBreak(time);
		}
	}

	for (int i = m_ActiveScenarios.size() - 1; i >= 0; i--)
	{
		IScenario* scenario = m_Scenarios[m_ActiveScenarios[i]];
		if (scenario->Update(dtS, world, scene))
		{
			Logger::LogEvent("Scenario [" + scenario->GetName() + "] Ended!");
			scenario->OnEnd();
			SetAsNonActive(m_ActiveScenarios[i]);
			m_ActiveScenarios.erase(m_ActiveScenarios.begin() + i);
		}
	}
}

void ScenarioManager::SetAsNonActive(int id)
{
	IScenario* scenario = m_Scenarios[id];
	m_NonActiveScenarios.push_back(id);
	scenario->SetTimeOfNextOutBreak(10);//Random::GenerateInt(scenario->GetCooldownTime(), scenario->GetMaxTimeBeforeOutbreak()));
}