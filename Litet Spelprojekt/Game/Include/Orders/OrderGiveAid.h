#pragma once
#include "OrderWalk.h"

class OrderGiveAid : public OrderWalk
{
public:
	OrderGiveAid(OrderGiveAid* other);
	OrderGiveAid(Crewmember* injuredMember);
	virtual ~OrderGiveAid();

	virtual void OnStarted(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept override;
	virtual void OnEnded(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept override;
	virtual bool OnUpdate(Scene* pScene, World* pWorld, Crew* pCrewMembers, float dtS) noexcept override;
	virtual bool CanBeStackedWithSameType() noexcept override;
	virtual std::string GetName() noexcept override;
	virtual bool ReadyToAbort() noexcept override;
	virtual bool IsIdleOrder() noexcept override;
	virtual void RunParallel() override;
	virtual bool CanExecuteIfHurt() noexcept override;

	virtual IOrder* Clone() noexcept override;
	virtual void BeginReplay(SceneGame* pScene, void* userData) noexcept override;

protected:
	Crewmember* m_pAiding;
	glm::ivec3 m_Target;

	//Replay
	uint32 m_InjuredMember;
};