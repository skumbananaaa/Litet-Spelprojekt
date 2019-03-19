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
	bool res = OrderWalk::OnUpdate(pScene, pWorld, pCrewMembers, dtS);
	Crewmember* pCrewmember = GetCrewMember();
	if (res)
	{
		pCrewmember->SetResting(!pCrewmember->IsAbleToWork());
		pCrewmember->SetIdling(false);
	}
	else if (!pCrewmember->IsAbleToWalk())
	{
		return true;
	}
	return res;
}

void OrderWalkMedicBay::OnEnded(Scene * pScene, World * pWorld, Crew * pCrewMembers) noexcept
{
	OrderWalk::OnEnded(pScene, pWorld, pCrewMembers);

	Crewmember* pCrewmember = GetCrewMember();
	pCrewmember->ReportPosition();
	Crewmember* assisted = pCrewmember->GetAssisting();
	if (assisted)
	{
		assisted->SetResting(true);
		pCrewmember->SetAssisting(nullptr);
		Logger::LogEvent(pCrewmember->GetName() + "bar " + assisted->GetName() + " till sjukstugan!", true);
	}
	else
	{
		if (pCrewmember->IsResting())
		{
			Logger::LogEvent(pCrewmember->GetName() + " gick till sjukstugan!", true);
		}
	}

	pCrewmember->ReportPosition();
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
