#pragma once
#include "IScenario.h"

#if defined(PRINT_CPU_DEBUG_DATA)
#include <System/CPUProfiler.h>
#endif

class ScenarioIceberg : public IScenario
{
public:
	ScenarioIceberg();
	virtual ~ScenarioIceberg();

	virtual void Init(World* pWorld) noexcept override;
	virtual void Release() noexcept override;
	virtual void OnStart(SceneGame* scene) noexcept override;
	virtual void OnEnd(SceneGame* scene)noexcept override;
	virtual void Escalate(const glm::ivec3& position, float severity) noexcept override;
	virtual void OnVisibilityChange(World* pWorld, SceneGame* pScene) noexcept override;
	virtual bool Update(float dtS, World* world, SceneGame* scene) noexcept override;
	virtual std::string GetName() noexcept override;
	virtual int32 GetCooldownTime() noexcept override;
	virtual int32 GetMaxTimeBeforeOutbreak() noexcept override;
	virtual bool IsComplete() noexcept override;
	int32 TestAgainstRay(const glm::vec3 ray, const glm::vec3 origin) noexcept;


private:
	glm::vec3 m_Position;
	glm::vec3 m_Target;
	glm::vec3 m_Direction;
	float m_Velocity;
	float m_TotalDistance;
	float m_DistanceTraveled;
	uint32 m_InstancesToSpawn;
	uint32 m_InstancesComplete;
	bool m_HasBounced;
	AudioSource* m_pAudioSourceExplosion;
};