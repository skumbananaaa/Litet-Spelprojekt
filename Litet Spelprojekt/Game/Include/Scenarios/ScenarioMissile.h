#pragma once
#include <World/Scenarios/IScenario.h>
#include "../GameObjectMissile.h"

#if defined(PRINT_CPU_DEBUG_DATA)
#include <System/CPUProfiler.h>
#endif

class ScenarioMissile : public IScenario
{
public:
	ScenarioMissile();
	virtual ~ScenarioMissile();

	virtual void Init(World* pWorld) noexcept override;
	virtual void OnStart(Scene* scene) noexcept override;
	virtual void OnEnd(Scene* scene)noexcept override;
	virtual void OnVisibilityChange(World* pWorld, Scene* pScene) noexcept override;
	virtual bool Update(float dtS, World* world, Scene* scene) noexcept override;
	virtual std::string GetName() noexcept override;
	virtual int32 GetCooldownTime() noexcept override;
	virtual int32 GetMaxTimeBeforeOutbreak() noexcept override;

private:
	GameObjectMissile* m_pGameObjectMissile;
};