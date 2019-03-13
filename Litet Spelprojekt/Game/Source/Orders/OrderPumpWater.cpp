#include "../../Include/Orders/OrderPumpWater.h"
#include <World/World.h>
OrderPumpWater::OrderPumpWater(uint32 roomToPump, glm::ivec3 pumpControl)
	: OrderWalk(pumpControl),
	m_RoomToPump(roomToPump)
{
}

OrderPumpWater::~OrderPumpWater()
{
}

void OrderPumpWater::OnStarted(Scene * pScene, World * pWorld, Crew * pCrewMembers) noexcept
{
	OrderWalk::OnStarted(pScene, pWorld, pCrewMembers);
}

void OrderPumpWater::OnEnded(Scene * pScene, World * pWorld, Crew * pCrewMembers) noexcept
{
}

bool OrderPumpWater::OnUpdate(Scene * pScene, World * pWorld, Crew * pCrewMembers, float dtS) noexcept
{
	if (OrderWalk::OnUpdate(pScene, pWorld, pCrewMembers, dtS))
	{
		pWorld->GetRoom(m_RoomToPump);
	}
	return false;
}

bool OrderPumpWater::CanBeStackedWithSameType() noexcept
{
	return false;
}

bool OrderPumpWater::HasPriority() noexcept
{
	return true;
}

std::string OrderPumpWater::GetName() noexcept
{
	return "Pump water from room " + std::to_string(m_RoomToPump);
}

bool OrderPumpWater::ReadyToAbort() noexcept
{
	return OrderWalk::ReadyToAbort();
}

bool OrderPumpWater::IsIdleOrder() noexcept
{
	return false;
}

void OrderPumpWater::RunParallel()
{
	OrderWalk::RunParallel();
}

bool OrderPumpWater::CanExecuteIfHurt() noexcept
{
	return false;
}
