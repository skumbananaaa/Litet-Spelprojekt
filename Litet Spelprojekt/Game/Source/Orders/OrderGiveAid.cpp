#include "../../Include/Orders/OrderGiveAid.h"
#include "../../Include/Crewmember.h"
OrderGiveAid::OrderGiveAid(Crewmember* injuredMember): OrderWalk(injuredMember->GetTile())
{
	m_pAiding = injuredMember;
	m_Target = injuredMember->GetTile();
}

OrderGiveAid::~OrderGiveAid()
{
}

void OrderGiveAid::StartOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept
{
	OrderWalk::StartOrder(pScene, pWorld, pCrewMembers);
}

bool OrderGiveAid::UpdateOrder(Scene * pScene, World * pWorld, Crew * pCrewMembers, float dtS) noexcept
{
	bool res = OrderWalk::UpdateOrder(pScene, pWorld, pCrewMembers, dtS);
	
	if (res)
	{
		res = m_pAiding->Heal(GetCrewMember()->GetSkillMedic(), dtS);
	}
	return res;
}

void OrderGiveAid::EndOrder(Scene * pScene, World * pWorld, Crew * pCrewMembers) noexcept
{
	OrderWalk::EndOrder(pScene, pWorld, pCrewMembers);

	m_pAiding = nullptr;
}

void OrderGiveAid::AbortOrder(Scene * pScene, World * pWorld, Crew * pCrewMembers) noexcept
{
	OrderWalk::AbortOrder(pScene, pWorld, pCrewMembers);
	m_pAiding = nullptr;
}

bool OrderGiveAid::AllowsMultipleOrders() noexcept
{
	return false;
}

std::string OrderGiveAid::GetName() noexcept
{
	return "Give Aid";
}

bool OrderGiveAid::ReadyToAbort() noexcept
{
	return OrderWalk::ReadyToAbort();
}

void OrderGiveAid::RunParallel()
{
	OrderWalk::RunParallel();
}
