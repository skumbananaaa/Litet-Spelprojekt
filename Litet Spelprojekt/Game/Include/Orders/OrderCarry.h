#pragma once
#include "OrderWalk.h"

class OrderCarry : public OrderWalk
{
public:
	OrderCarry(OrderCarry* other);
	OrderCarry(Crewmember* inNeedOfAssist);
	virtual ~OrderCarry();

	virtual void OnStarted(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept override;
	virtual void OnEnded(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept override;
	virtual bool OnUpdate(Scene* pScene, World* pWorld, Crew* pCrewMembers, float dtS) noexcept override;
	virtual bool CanBeStackedWithSameType() noexcept override;
	virtual bool HasPriority() noexcept override;
	virtual std::string GetName() noexcept override;
	virtual bool ReadyToAbort() noexcept override;
	virtual bool IsIdleOrder() noexcept override;
	virtual void RunParallel() override;
	virtual bool CanExecuteIfHurt() noexcept override;

	virtual IOrder* Clone() noexcept override;
	virtual void InitClone(SceneGame* pScene, void* userData) noexcept override;

protected:
	Crewmember* m_pCarrying;

	//Replay
	uint32 m_InNeedOfAssist;
};
