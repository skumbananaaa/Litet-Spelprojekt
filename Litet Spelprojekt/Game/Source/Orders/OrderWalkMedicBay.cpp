#include "../../Include/Orders/OrderWalkMedicBay.h"
#include <World/World.h>
OrderWalkMedicBay::OrderWalkMedicBay(World* pWorld): OrderWalk(glm::ivec3())
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
	if (OrderWalk::OnUpdate(pScene, pWorld, pCrewMembers, dtS))
	{
		// Make it so that a crewmember goes to bed.
	}
	return false;
}
/*
void OrderWalkMedicBay::EndOrder(Scene * pScene, World * pWorld, Crew * pCrewMembers) noexcept
{
	OrderWalk::EndOrder(pScene, pWorld, pCrewMembers);
}

void OrderWalkMedicBay::AbortOrder(Scene * pScene, World * pWorld, Crew * pCrewMembers) noexcept
{
	OrderWalk::AbortOrder(pScene, pWorld, pCrewMembers);
}

bool OrderWalkMedicBay::AllowsMultipleOrders() noexcept
{
	return OrderWalk::AllowsMultipleOrders();
}*/

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
