#pragma once
#include "IOrder.h"


class OrderCloseDoor : public IOrder
{
protected:

public:
	OrderCloseDoor();
	virtual ~OrderCloseDoor();

	virtual void StartOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept override;
	virtual bool UpdateOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers, float dtS) noexcept override;
	virtual void EndOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept override;
};