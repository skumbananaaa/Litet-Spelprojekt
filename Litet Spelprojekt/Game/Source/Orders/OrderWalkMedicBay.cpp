#include "../../Include/Orders/OrderWalkMedicBay.h"
#include "../../Include/Crewmember.h"
#include <World/World.h>
#include "../../Include/Game.h"

OrderWalkMedicBay::OrderWalkMedicBay(OrderWalkMedicBay* other)
	: OrderWalk(other)
{
}

OrderWalkMedicBay::OrderWalkMedicBay(const glm::ivec3& currtile)
	: OrderWalk(Game::GetGame()->m_pSceneGame->GetWorld()->FindClosestRoomInInterval(SICKBAY_INTERVAL_START, SICKBAY_INTERVAL_END, currtile))
{
}

OrderWalkMedicBay::~OrderWalkMedicBay()
{
}

/*void OrderWalkMedicBay::StartOrder(Scene * pScene, World * pWorld, Crew * pCrewMembers) noexcept
{
	OrderWalk::StartOrder(pScene, pWorld, pCrewMembers);
}
*/
bool OrderWalkMedicBay::OnUpdate(Scene * pScene, World * pWorld, Crew * pCrewMembers, float dtS) noexcept
{
	/*if (OrderWalk::OnUpdate(pScene, pWorld, pCrewMembers, dtS))
	{
		// Make it so that a crewmember goes to bed.
	}*/
	return OrderWalk::OnUpdate(pScene, pWorld, pCrewMembers, dtS);
}

void OrderWalkMedicBay::OnEnded(Scene * pScene, World * pWorld, Crew * pCrewMembers) noexcept
{
	OrderWalk::OnEnded(pScene, pWorld, pCrewMembers);

	GetCrewMember()->ReportPosition();
	GetCrewMember()->SetResting(true);
}
/*
void OrderWalkMedicBay::AbortOrder(Scene * pScene, World * pWorld, Crew * pCrewMembers) noexcept
{
	OrderWalk::AbortOrder(pScene, pWorld, pCrewMembers);
}

bool OrderWalkMedicBay::AllowsMultipleOrders() noexcept
{
	return OrderWalk::AllowsMultipleOrders();
}*/

bool OrderWalkMedicBay::HasPriority() noexcept
{
	return true;
}

std::string OrderWalkMedicBay::GetName() noexcept
{
	return "Go to medic bay!";
}

IOrder* OrderWalkMedicBay::Clone() noexcept
{
	return new OrderWalkMedicBay(this);
}

void OrderWalkMedicBay::BeginReplay(SceneGame * pScene, void * userData) noexcept
{
	OrderWalk::BeginReplay(pScene, userData);
}

/*bool OrderWalkMedicBay::ReadyToAbort() noexcept
{
	return OrderWalk::ReadyToAbort();
}

void OrderWalkMedicBay::RunParallel()
{
	OrderWalk::RunParallel();
}*/
