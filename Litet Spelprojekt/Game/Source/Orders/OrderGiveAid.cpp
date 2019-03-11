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

void OrderGiveAid::OnStarted(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept
{
	OrderWalk::OnStarted(pScene, pWorld, pCrewMembers);
}

bool OrderGiveAid::OnUpdate(Scene * pScene, World * pWorld, Crew * pCrewMembers, float dtS) noexcept
{
	bool res = OrderWalk::OnUpdate(pScene, pWorld, pCrewMembers, dtS);
	uint32 healLevel = 2;
	if (GetCrewMember()->GetGroup() == MEDIC)
	{
		healLevel = 5;
	}
	if (res)
	{
		res = m_pAiding->Heal(healLevel, dtS);
	}
	return res;
}

void OrderGiveAid::OnEnded(Scene * pScene, World * pWorld, Crew * pCrewMembers) noexcept
{
	OrderWalk::OnEnded(pScene, pWorld, pCrewMembers);
	m_pAiding = nullptr;
}

bool OrderGiveAid::CanBeStackedWithSameType() noexcept
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

bool OrderGiveAid::IsIdleOrder() noexcept
{
	return false;
}

void OrderGiveAid::RunParallel()
{
	OrderWalk::RunParallel();
}

bool OrderGiveAid::CanExecuteIfHurt() noexcept
{
	return false;
}
