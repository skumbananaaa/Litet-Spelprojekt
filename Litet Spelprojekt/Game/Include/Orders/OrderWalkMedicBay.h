#pragma once
#include "OrderWalk.h"

class OrderWalkMedicBay : public OrderWalk
{
public:
	OrderWalkMedicBay(World* pWorld);
	virtual ~OrderWalkMedicBay() override;

/*	virtual void StartOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept override;
	virtual bool UpdateOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers, float dtS) noexcept override;
	virtual void EndOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept override;
	virtual void AbortOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept override;
	virtual bool AllowsMultipleOrders() noexcept override;*/
	virtual std::string GetName() noexcept override;
	/*virtual bool ReadyToAbort() noexcept override;
	virtual void RunParallel() override;*/
};