#pragma once
#include <EnginePch.h>
#include <World/World.h>
#include <Graphics/Scene.h>

class API IScenario
{
protected:
	IScenario();

public:
	virtual ~IScenario();
	virtual void Init(World* pWorld) noexcept = 0;
	virtual void OnStart(Scene* scene) noexcept = 0;
	virtual void OnEnd(Scene* scene)noexcept = 0;
	virtual void OnVisibilityChange(World* pWorld, Scene* pScene) = 0;
	virtual bool Update(float dtS, World* pWorld, Scene* pScene) noexcept = 0;
	virtual std::string GetName() noexcept = 0;
	virtual int32 GetCooldownTime() noexcept = 0;
	virtual int32 GetMaxTimeBeforeOutbreak() noexcept = 0;

	float GetTimeOfNextOutBreak() const noexcept;
	void SetTimeOfNextOutBreak(float time) noexcept;

private:
	float m_TimeOfNextOutBreak;
};