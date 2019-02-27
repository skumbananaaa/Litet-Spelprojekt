#pragma once
#include <World/Scenarios/IScenario.h>

class ScenarioManager
{
public:
	static uint32 RegisterScenario(IScenario* scenario) noexcept;
	static void Release() noexcept;
	static void OnVisibilityChange(World* pWorld, Scene* pScene, const std::vector<uint32>& activeRooms) noexcept;
	static void Update(float dtS, World* world, Scene* scene, const std::vector<uint32>& activeRooms) noexcept;
	static void SetEnabledScenarios(const std::vector<int> ids) noexcept;

private:
	static void SetAsNonActive(int id);

	static std::vector<IScenario*> s_Scenarios;
	static std::vector<int32> s_ActiveScenarios;
	static std::vector<int32> s_NonActiveScenarios;
};