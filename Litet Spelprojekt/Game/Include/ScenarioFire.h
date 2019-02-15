#pragma once
#include <World/IScenario.h>

class ScenarioFire : public IScenario
{
public:
	ScenarioFire();

	virtual void OnStart() noexcept override;
	virtual void OnEnd()noexcept override;
	virtual bool Update(float dtS) noexcept override;
	virtual std::string GetName() noexcept override;
	virtual int32 GetCooldownTime() noexcept override;
	virtual int32 GetMaxTimeBeforeOutbreak() noexcept override;
};