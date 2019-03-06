#pragma once
#include "OrderWalk.h"

class OrderGiveAid : public OrderWalk
{
protected:

public:
	OrderGiveAid(Crewmember* injuredMember);
	virtual ~OrderGiveAid();

	virtual void StartOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept override;
	virtual bool UpdateOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers, float dtS) noexcept override;
	virtual void EndOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept override;
	virtual void AbortOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept override;
	virtual bool AllowsMultipleOrders() noexcept override;
	virtual std::string GetName() noexcept override;
	virtual bool ReadyToAbort() noexcept override;
	virtual bool IsIdleOrder() noexcept override;
	virtual void RunParallel() override;
	virtual bool CanExecuteIfHurt() noexcept override;
protected:
	Crewmember* m_pAiding;
	glm::ivec3 m_Target;
};