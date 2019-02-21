#pragma once
#include <EnginePch.h>
#include <World/World.h>
#include <Graphics/Scene.h>

class API IScenario
{
protected:
	IScenario() : m_TimeOfNextOutBreak(0) {};

public:
	virtual ~IScenario() {};
	virtual void OnStart(Scene* scene) noexcept = 0;
	virtual void OnEnd()noexcept = 0;
	virtual void OnVisibilityChange(World* pWorld, Scene* pScene, const std::vector<uint32>& activeRooms) = 0;
	virtual bool Update(float dtS, World* pWorld, Scene* pScene, const std::vector<uint32>& activeRooms) noexcept = 0;
	virtual std::string GetName() noexcept = 0;
	virtual int32 GetCooldownTime() noexcept = 0;
	virtual int32 GetMaxTimeBeforeOutbreak() noexcept = 0;

	float GetTimeOfNextOutBreak() const noexcept
	{
		return m_TimeOfNextOutBreak;
	};

	void SetTimeOfNextOutBreak(float time) noexcept
	{
		m_TimeOfNextOutBreak = time;
	};

private:
	float m_TimeOfNextOutBreak;
};