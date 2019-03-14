#pragma once
#include "OrderWalk.h"

class OrderPumpWater : public OrderWalk
{
public:

	OrderPumpWater(uint32 roomToPump, glm::ivec3 pumpControl);
	virtual ~OrderPumpWater();

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

private:
	uint32 m_RoomToPump;
	bool m_Pumping;
};