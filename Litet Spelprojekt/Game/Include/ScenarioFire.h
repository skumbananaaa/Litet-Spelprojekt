#pragma once
#include <World/IScenario.h>

class ScenarioFire : public IScenario
{
public:
	ScenarioFire(World* world);
	~ScenarioFire();

	virtual void OnStart() noexcept override;
	virtual void OnEnd()noexcept override;
	virtual bool Update(float dtS, World* world, Scene* scene) noexcept override;
	virtual std::string GetName() noexcept override;
	virtual int32 GetCooldownTime() noexcept override;
	virtual int32 GetMaxTimeBeforeOutbreak() noexcept override;

private:
	World* m_pWorld;

	const uint32* const** m_pppMap;

	void CheckFire(float dtS, const glm::ivec3& offset, const glm::ivec3& origin, Scene* scene);
	void CheckSmoke(float dtS, const glm::ivec3& offset, const glm::ivec3& origin, float amount);

	void SpreadFire(float dtS, const glm::ivec3& offset, const glm::ivec3& origin, Scene* scene, float rateOfSpread);

	// x, y, z = x, level, z
	std::vector<glm::vec3> m_OnFire;
};