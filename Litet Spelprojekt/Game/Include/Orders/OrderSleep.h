#pragma once
#include "OrderWalk.h"

class GameObject;

class OrderSleep : public OrderWalk
{
public:
	OrderSleep(OrderSleep* other);
	OrderSleep(const glm::ivec3& bedTile, GameObject* pBed);
	virtual ~OrderSleep();

	virtual void OnStarted(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept override;
	virtual void OnEnded(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept override;
	virtual bool OnUpdate(Scene* pScene, World* pWorld, Crew* pCrewMembers, float dtS) noexcept override;
	virtual bool CanBeStackedWithSameType() noexcept override;
	virtual std::string GetName() noexcept override;
	virtual bool IsIdleOrder() noexcept;

	virtual IOrder* Clone() noexcept override;
	virtual void BeginReplay(SceneGame* pScene, void* userData) noexcept override;

private:
	GameObject* m_pBed;
	glm::vec3 m_Position;
	float m_Timer;
	bool m_IsAtBed;
	bool m_Up;

	//Replay
	std::string m_Name;
};