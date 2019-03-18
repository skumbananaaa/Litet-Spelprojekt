#pragma once
#include <EnginePch.h>
#include <World/World.h>
#include "../Scenes/SceneGame.h"

class IScenario
{
protected:
	IScenario();

public:
	virtual ~IScenario();
	virtual void Init(World* pWorld) noexcept = 0;
	virtual void Release() noexcept = 0;
	virtual void OnStart(SceneGame* scene) noexcept = 0;
	virtual void OnEnd(SceneGame* scene)noexcept = 0;
	virtual void Escalate(const glm::ivec3& position) noexcept = 0;
	virtual void OnVisibilityChange(World* pWorld, SceneGame* pScene) = 0;
	virtual bool Update(float dtS, World* pWorld, SceneGame* pScene) noexcept = 0;
	virtual std::string GetName() noexcept = 0;
	virtual int32 GetCooldownTime() noexcept = 0;
	virtual int32 GetMaxTimeBeforeOutbreak() noexcept = 0;
	virtual bool IsComplete() noexcept = 0;

	float GetTimeOfNextOutBreak() const noexcept;
	void SetTimeOfNextOutBreak(float time) noexcept;

private:
	float m_TimeOfNextOutBreak;
};