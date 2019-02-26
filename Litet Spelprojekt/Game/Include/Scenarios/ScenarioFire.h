#pragma once
#include <World/Scenarios/IScenario.h>

#if defined(PRINT_CPU_DEBUG_DATA)
#include <System/CPUProfiler.h>
#endif

class ScenarioFire : public IScenario
{
public:
	ScenarioFire(World* world);
	virtual ~ScenarioFire();

	virtual void OnStart(Scene* scene) noexcept override;
	virtual void OnEnd()noexcept override;
	virtual void OnVisibilityChange(World* pWorld, Scene* pScene, const std::vector<uint32>& activeRooms) noexcept override;
	virtual bool Update(float dtS, World* world, Scene* scene, const std::vector<uint32>& activeRooms) noexcept override;
	virtual std::string GetName() noexcept override;
	virtual int32 GetCooldownTime() noexcept override;
	virtual int32 GetMaxTimeBeforeOutbreak() noexcept override;

	void ShowInRoom(uint32 roomID) noexcept;
private:
	World* m_pWorld;

	const uint32* const** m_pppMap;

	void CheckFire(float dtS, const glm::ivec3& offset, const glm::ivec3& origin, Scene* scene);
	bool CheckSmoke(float dtS, const glm::ivec3& offset, const glm::ivec3& origin, float amount, Scene* scene);

	// x, y, z = x, level, z
	std::vector<glm::ivec3> m_OnFire;
	std::vector<glm::ivec3> m_Smoke;
};