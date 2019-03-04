#include <EnginePch.h>
#include "../Scenarios/Fir"

IScenario::IScenario() : m_TimeOfNextOutBreak(0)
{
}

IScenario::~IScenario()
{
}

float IScenario::GetTimeOfNextOutBreak() const noexcept
{
	return m_TimeOfNextOutBreak;
}

void IScenario::SetTimeOfNextOutBreak(float time) noexcept
{
	m_TimeOfNextOutBreak = time;
}
