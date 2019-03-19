#pragma once
#include "OrderWalk.h"

class GameObjectDoor;

class OrderDoor : public OrderWalk 
{
public:
	OrderDoor(OrderDoor* other);
	OrderDoor(GameObjectDoor* door, const glm::ivec3& doorTile, bool open);
	virtual ~OrderDoor();

	virtual void OnStarted(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept override;
	virtual void OnEnded(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept override;
	virtual bool OnUpdate(Scene* pScene, World* pWorld, Crew* pCrewMembers, float dtS) noexcept override;
	virtual bool CanBeStackedWithSameType() noexcept override;
	virtual std::string GetName() noexcept override;
	virtual bool IsIdleOrder() noexcept override;
	virtual bool CanExecuteIfHurt() noexcept override;

	virtual IOrder* Clone() noexcept override;
	virtual void InitClone(SceneGame* pScene, void* userData) noexcept override;

private:
	GameObjectDoor* m_pGameObjectDoor;
	bool m_Open;

	//Replay
	std::string m_DoorName;
};