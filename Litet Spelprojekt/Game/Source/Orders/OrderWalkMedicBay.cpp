#include "../../Include/Orders/OrderWalkMedicBay.h"
#include "../../Include/Crewmember.h"
#include <World/World.h>
OrderWalkMedicBay::OrderWalkMedicBay(World* pWorld, const glm::ivec3& currtile)
	: OrderWalk(pWorld->FindClosestRoomInInterval(SICKBAY_INTERVAL_START, SICKBAY_INTERVAL_END, currtile))
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

	Crewmember* pMember = GetCrewMember();
	pMember->ReportPosition();
	pMember->SetResting(!pMember->IsAbleToWork());
	Crewmember* assisted = pMember->GetAssisting();
	if (assisted)
	{
		assisted->SetResting(true);
		pMember->SetAssisting(nullptr);
	}

	Logger::LogEvent(GetCrewMember()->GetName() + " gick till sjukstugan!", true);
	GetCrewMember()->ReportPosition();
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

/*bool OrderWalkMedicBay::ReadyToAbort() noexcept
{
	return OrderWalk::ReadyToAbort();
}

void OrderWalkMedicBay::RunParallel()
{
	OrderWalk::RunParallel();
}*/
