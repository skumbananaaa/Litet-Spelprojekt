#pragma once
#include <EnginePch.h>

class API IScenario
{
protected:
	IScenario() : m_TimeOfNextOutBreak(0) {};

public:
	virtual void OnStart() noexcept = 0;
	virtual void OnEnd()noexcept = 0;
	virtual bool Update(float dtS) noexcept = 0;
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