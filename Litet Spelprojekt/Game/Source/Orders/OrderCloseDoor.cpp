#include "../../Include/Orders/OrderCloseDoor.h"
#include "../../Include/GameObjectDoor.h"
#include "../../Include/Crewmember.h"

OrderDoor::OrderDoor(GameObjectDoor* door, const glm::ivec3& doorTile, bool open) : OrderWalk(doorTile)
{
	m_pGameObjectDoor = door;
	m_Open = open;
}

OrderDoor::~OrderDoor()
{
}

void OrderDoor::StartOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept
{
	OrderWalk::StartOrder(pScene, pWorld, pCrewMembers);
}

bool OrderDoor::UpdateOrder(Scene * pScene, World * pWorld, Crew * pCrewMembers, float dtS) noexcept
{
	if (m_pGameObjectDoor->IsOpen() == m_Open || m_pGameObjectDoor->IsClosed() == !m_Open)
	{
		return true;
	}
	else if (OrderWalk::UpdateOrder(pScene, pWorld, pCrewMembers, dtS))
	{
		m_pGameObjectDoor->SetOpen(m_Open);
		return true;
	}
	return false;
}

void OrderDoor::EndOrder(Scene * pScene, World * pWorld, Crew * pCrewMembers) noexcept
{
	OrderWalk::EndOrder(pScene, pWorld, pCrewMembers);
}

void OrderDoor::AbortOrder(Scene * pScene, World * pWorld, Crew * pCrewMembers) noexcept
{
	OrderWalk::AbortOrder(pScene, pWorld, pCrewMembers);
}

bool OrderDoor::AllowsMultipleOrders() noexcept
{
	return true;
}

std::string OrderDoor::GetName() noexcept
{
	return "Close door";
}

bool OrderDoor::IsIdleOrder() noexcept
{
	return false;
}

bool OrderDoor::CanExecuteIfHurt() noexcept
{
	return false;
}



