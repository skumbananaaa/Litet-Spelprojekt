#pragma once
#include "OrderWalk.h"
class OrderCarry : public OrderWalk
{
protected:

public:
	OrderCarry(Crewmember* InNeedOfAssist);
	virtual ~OrderCarry();

	virtual void StartOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept;
	virtual void EndOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept;
	virtual bool UpdateOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers, float dtS) noexcept;
	virtual void AbortOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept;
	virtual bool AllowsMultipleOrders() noexcept;
	virtual std::string GetName() noexcept;
	virtual bool ReadyToAbort() noexcept;

protected:
	Crewmember* m_pCarrying;
};
