#pragma once
#include "OrderWalk.h"

class GameObjectDoor;


class OrderDoor : public OrderWalk 
{
public:
	OrderDoor(GameObjectDoor* door, glm::ivec3 doorTile, bool open);
	virtual ~OrderDoor();

	virtual void StartOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept override;
	virtual bool UpdateOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers, float dtS) noexcept override;
	virtual void EndOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept override;
	virtual void AbortOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept override;
	virtual bool AllowsMultipleOrders() noexcept override;
	virtual std::string GetName() noexcept override;
	virtual bool IsIdleOrder() noexcept;

private:
	GameObjectDoor* m_pGameObjectDoor;
	bool m_Open;
};