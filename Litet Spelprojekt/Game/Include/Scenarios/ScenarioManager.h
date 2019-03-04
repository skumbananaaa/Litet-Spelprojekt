#pragma once
#include "IScenario.h"

class ScenarioManager
{
public:
	static uint32 RegisterScenario(IScenario* scenario) noexcept;
	static void Release() noexcept;
	static void OnVisibilityChange(World* pWorld, SceneGame* pScene, const std::vector<uint32>& activeRooms) noexcept;
	static void Update(float dtS, World* world, SceneGame* scene, const std::vector<uint32>& activeRooms) noexcept;
	static void SetEnabledScenarios(const std::vector<int> ids) noexcept;
	static bool StartScenario(int32 index) noexcept;
	static void Escalate(int32 index, const glm::ivec3& position) noexcept;
	static void Init(World* pWorld);
	static const std::vector<IScenario*> GetScenarios() noexcept;

private:
	static void SetAsNonActive(int id);

	static std::vector<IScenario*> s_Scenarios;
	static std::vector<int32> s_ActiveScenarios;
	static std::vector<int32> s_NonActiveScenarios;
};