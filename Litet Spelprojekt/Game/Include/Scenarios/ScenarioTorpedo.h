#pragma once
#include "IScenario.h"
#include "../GameObjectTorpedo.h"

#if defined(PRINT_CPU_DEBUG_DATA)
#include <System/CPUProfiler.h>
#endif

class ScenarioTorpedo : public IScenario, public IMeshListener
{
public:
	ScenarioTorpedo();
	virtual ~ScenarioTorpedo();

	virtual void Init(World* pWorld) noexcept override;
	virtual void OnStart(SceneGame* scene) noexcept override;
	virtual void OnEnd(SceneGame* scene)noexcept override;
	virtual void Escalate(const glm::ivec3& position) noexcept override;
	virtual void OnVisibilityChange(World* pWorld, SceneGame* pScene) noexcept override;
	virtual bool Update(float dtS, World* world, SceneGame* scene) noexcept override;
	virtual void OnMeshEmitterKilled(MeshEmitter* emitter) override;
	virtual std::string GetName() noexcept override;
	virtual int32 GetCooldownTime() noexcept override;
	virtual int32 GetMaxTimeBeforeOutbreak() noexcept override;

private:
	GameObjectTorpedo* m_pGameObjectTorpedo;
	glm::vec3 m_Target;
	AudioSource* pAudioSourceExplosion;
};