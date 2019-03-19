#pragma once
#include "OrderWalk.h"

class GameObject;

class OrderCook : public OrderWalk
{
public:
	OrderCook(OrderCook* other);
	OrderCook(const glm::ivec3& ovenTile, GameObject* pOven);
	virtual ~OrderCook();

	virtual void OnStarted(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept override;
	virtual void OnEnded(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept override;
	virtual bool OnUpdate(Scene* pScene, World* pWorld, Crew* pCrewMembers, float dtS) noexcept override;
	virtual bool CanBeStackedWithSameType() noexcept override;
	virtual std::string GetName() noexcept override;
	virtual bool IsIdleOrder() noexcept;

	virtual IOrder* Clone() noexcept override;
	virtual void InitClone(SceneGame* pScene, void* userData) noexcept override;

private:
	GameObject* m_pOven;
	glm::vec3 m_Position;
	float m_Timer;
	bool m_IsAtOven;

	//Replay
	std::string m_Name;
};