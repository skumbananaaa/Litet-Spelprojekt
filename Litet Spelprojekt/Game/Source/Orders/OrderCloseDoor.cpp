#include "../../Include/Orders/OrderCloseDoor.h"
#include <World/GameObjectDoor.h>

OrderCloseDoor::OrderCloseDoor(GameObjectDoor* door) : OrderWalk(door->GetPosition())
{
	m_pGameObjectDoor = door;
}

OrderCloseDoor::~OrderCloseDoor()
{
}

void OrderCloseDoor::StartOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept
{
	OrderWalk::StartOrder(pScene, pWorld, pCrewMembers);
}

bool OrderCloseDoor::UpdateOrder(Scene * pScene, World * pWorld, Crew * pCrewMembers, float dtS) noexcept
{
	if (!m_IsPathReady)
	{
		return false;
	}
	if (!m_pGameObjectDoor->IsOpen())
	{
		return true;
	}
	else if (FollowPath(dtS))
	{
		m_pGameObjectDoor->SetOpen(false);
		return true;
	}
	return false;
}

void OrderCloseDoor::EndOrder(Scene * pScene, World * pWorld, Crew * pCrewMembers) noexcept
{
	OrderWalk::EndOrder(pScene, pWorld, pCrewMembers);
}

void OrderCloseDoor::AbortOrder(Scene * pScene, World * pWorld, Crew * pCrewMembers) noexcept
{
	OrderWalk::AbortOrder(pScene, pWorld, pCrewMembers);
}

bool OrderCloseDoor::AllowsMultipleOrders() noexcept
{
	return true;
}

std::string OrderCloseDoor::GetName() noexcept
{
	return "Close door";
}



