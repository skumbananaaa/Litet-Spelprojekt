#pragma once
#include <World/Scenarios/IScenario.h>

class ScenarioManager
{
public:
	static uint32 RegisterScenario(IScenario* scenario) noexcept;
	static void Release() noexcept;
	static void OnVisibilityChange(World* pWorld, Scene* pScene) noexcept;
	static void Update(float dtS, World* world, Scene* scene) noexcept;
	static void SetEnabledScenarios(const std::vector<int>& ids) noexcept;
	static void Init(World* pWorld);
	static const std::vector<IScenario*>& GetScenarios() noexcept;

private:
	static void SetAsNonActive(int id);

	static std::vector<IScenario*> s_Scenarios;
	static std::vector<int32> s_ActiveScenarios;
	static std::vector<int32> s_NonActiveScenarios;
};