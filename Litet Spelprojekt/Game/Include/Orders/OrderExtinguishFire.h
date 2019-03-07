#pragma once
#include "OrderWalk.h"

#define TIME_TO_EQUIP_GEAR 5.0f
#define FIRE_EXTINGUISH_TEMP_REDUCTION 20.0f

class OrderExtinguishFire : public OrderWalk
{
public:
	OrderExtinguishFire(const glm::ivec3& roomTile, const glm::ivec3& burningTile, uint32 roomBurningId, bool hasGearEquipped);
	virtual ~OrderExtinguishFire();

	virtual void StartOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept override;
	virtual bool UpdateOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers, float dtS) noexcept override;
	virtual void EndOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept override;
	virtual void AbortOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept override;
	virtual bool AllowsMultipleOrders() noexcept override;
	virtual std::string GetName() noexcept override;
	virtual bool IsIdleOrder() noexcept override;
	virtual bool CanExecuteIfHurt() noexcept override;

private:
	uint32 m_RoomBurningId;
	float m_EquippingGearTimer;
	glm::ivec3 m_RoomTile;
	glm::ivec3 m_BurningTile;
};