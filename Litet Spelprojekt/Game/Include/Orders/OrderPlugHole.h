#pragma once
#include "OrderWalk.h"
#define EQUIPTIME 5.0f
#define PLUGTIME 5.0f
class OrderPlugHole : public OrderWalk
{
public:
	OrderPlugHole(OrderPlugHole* other);
	OrderPlugHole(const glm::ivec3& roomTile, const glm::ivec3& holeTile, bool hasGearEquipped);
	virtual ~OrderPlugHole();
	virtual void OnStarted(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept override;
	virtual void OnEnded(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept override;
	virtual bool OnUpdate(Scene* pScene, World* pWorld, Crew* pCrewMembers, float dtS) noexcept override;
	virtual bool CanBeStackedWithSameType() noexcept override;
	virtual std::string GetName() noexcept override;
	virtual bool IsIdleOrder() noexcept;

	virtual IOrder* Clone() noexcept override;

private:
	uint32 m_RoomFloodingId;
	glm::ivec3 m_RoomTile;
	glm::ivec3 m_HoleTile;
	float m_EquippingGearTimer;
	bool m_PluggingHole;
	float m_PluggingTimer;
	bool m_HolePlugged;
};