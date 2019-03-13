#include "OrderPumpWater.h"
#include <World/World.h>
OrderPumpWater::OrderPumpWater(uint32 roomToPump, glm::ivec3 pumpControl)
	: OrderWalk(pumpControl),
	m_RoomToPump(roomToPump)
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
