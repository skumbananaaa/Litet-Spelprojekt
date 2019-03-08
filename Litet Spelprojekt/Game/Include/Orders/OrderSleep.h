#pragma once
#include "OrderWalk.h"

class GameObject;

class OrderSleep : public OrderWalk
{
public:
	OrderSleep(const glm::ivec3& bedTile, GameObject* pBed);
	virtual ~OrderSleep();

	virtual void StartOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept override;
	virtual bool UpdateOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers, float dtS) noexcept override;
	virtual void EndOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept override;
	virtual void AbortOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept override;
	virtual bool AllowsMultipleOrders() noexcept override;
	virtual std::string GetName() noexcept override;
	virtual bool ReadyToAbort() noexcept override;
	virtual bool IsIdleOrder() noexcept;

private:
	GameObject* m_pBed;
	glm::vec3 m_Position;
	float m_Timer;
	bool m_IsAtBed;
};