#pragma once
#include "IOrder.h"
#include "OrderWalk.h"

class GameObjectDoor;


class OrderCloseDoor : public OrderWalk 
{
public:
	OrderCloseDoor(GameObjectDoor* door);
	virtual ~OrderCloseDoor();

	virtual void StartOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept override;
	virtual bool UpdateOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers, float dtS) noexcept override;
	virtual void EndOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept override;
	virtual void AbortOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept override;
	virtual bool AllowsMultipleOrders() noexcept override;
	virtual std::string GetName() noexcept override;

private:
	GameObjectDoor* m_pGameObjectDoor;
};