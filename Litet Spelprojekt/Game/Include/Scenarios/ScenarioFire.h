#pragma once
#include <World/Scenarios/IScenario.h>

class ScenarioFire : public IScenario
{
public:
	ScenarioFire(World* world);
	~ScenarioFire();

	virtual void OnStart() noexcept override;
	virtual void OnEnd()noexcept override;
	virtual void OnVisibilityChange(World* pWorld, Scene* pScene, const std::vector<uint32>& activeRooms) noexcept override;
	virtual bool Update(float dtS, World* world, Scene* scene, const std::vector<uint32>& activeRooms) noexcept override;
	virtual std::string GetName() noexcept override;
	virtual int32 GetCooldownTime() noexcept override;
	virtual int32 GetMaxTimeBeforeOutbreak() noexcept override;

private:
	World* m_pWorld;

	const uint32* const** m_pppMap;

	void CheckFire(float dtS, const glm::ivec3& offset, const glm::ivec3& origin, Scene* scene);
	bool CheckSmoke(float dtS, const glm::ivec3& offset, const glm::ivec3& origin, float amount);

	// x, y, z = x, level, z
	std::vector<glm::ivec3> m_OnFire;
	std::vector<glm::ivec3> m_Smoke;
};