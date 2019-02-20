#pragma once
#include <World/Scenarios/IScenario.h>

class ScenarioFire : public IScenario
{
public:
	ScenarioFire();

	virtual void OnStart() noexcept override;
	virtual void OnEnd()noexcept override;
	virtual void OnVisibilityChange(World* pWorld, Scene* pScene, const std::vector<uint32>& activeRooms) override;
	virtual bool Update(float dtS, World* pWorld, Scene* pScene, const std::vector<uint32>& activeRooms) noexcept override;
	virtual std::string GetName() noexcept override;
	virtual int32 GetCooldownTime() noexcept override;
	virtual int32 GetMaxTimeBeforeOutbreak() noexcept override;
};