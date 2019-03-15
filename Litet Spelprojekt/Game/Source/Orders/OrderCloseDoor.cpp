#include "../../Include/Orders/OrderCloseDoor.h"
#include "../../Include/GameObjectDoor.h"
#include "../../Include/Crewmember.h"

OrderDoor::OrderDoor(GameObjectDoor* door, const glm::ivec3& doorTile, bool open)
	: OrderWalk(doorTile)
{
	m_pGameObjectDoor = door;
	m_Open = open;
}

OrderDoor::~OrderDoor()
{
}

void OrderDoor::OnStarted(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept
{
	OrderWalk::OnStarted(pScene, pWorld, pCrewMembers);
}

bool OrderDoor::OnUpdate(Scene * pScene, World * pWorld, Crew * pCrewMembers, float dtS) noexcept
{
	uint32 shipNr = GetCrewMember()->GetShipNumber();

	if (m_pGameObjectDoor->IsOpen() == m_Open || m_pGameObjectDoor->IsClosed() == !m_Open || (!m_pGameObjectDoor->AccessRequest(shipNr) && m_Open) || (!m_pGameObjectDoor->RemoveFromQueue(shipNr) && !m_Open))
	{
		m_pGameObjectDoor->RemoveFromQueue(shipNr);
		return true;
	}
	else if (OrderWalk::OnUpdate(pScene, pWorld, pCrewMembers, dtS))
	{
		m_pGameObjectDoor->SetOpen(m_Open);
		return true;
	}
	return false;
}

void OrderDoor::OnEnded(Scene * pScene, World * pWorld, Crew * pCrewMembers) noexcept
{
	OrderWalk::OnEnded(pScene, pWorld, pCrewMembers);
}

bool OrderDoor::CanBeStackedWithSameType() noexcept
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
