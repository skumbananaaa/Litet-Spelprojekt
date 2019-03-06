#include "../../Include/Orders/OrderCarry.h"
#include "../../Include/Crew.h"

OrderCarry::OrderCarry(Crewmember * inNeedOfAssist) : OrderWalk(inNeedOfAssist->GetTile())
{
	m_pCarrying = inNeedOfAssist;
	GetCrewMember()->SetAssisting(nullptr);
}

OrderCarry::~OrderCarry()
{
}

void OrderCarry::StartOrder(Scene * pScene, World * pWorld, Crew * pCrewMembers) noexcept
{
	OrderWalk::StartOrder(pScene, pWorld, pCrewMembers);
}

void OrderCarry::EndOrder(Scene * pScene, World * pWorld, Crew * pCrewMembers) noexcept
{
	OrderWalk::EndOrder(pScene, pWorld, pCrewMembers);
	//GetCrewMember()->SetAssisting(nullptr);
}

bool OrderCarry::UpdateOrder(Scene * pScene, World * pWorld, Crew * pCrewMembers, float dtS) noexcept
{
	if (OrderWalk::UpdateOrder(pScene, pWorld, pCrewMembers, dtS))
	{
		GetCrewMember()->SetAssisting(m_pCarrying);
		return true;
	}
	return false;
}

void OrderCarry::AbortOrder(Scene * pScene, World * pWorld, Crew * pCrewMembers) noexcept
{
	GetCrewMember()->SetAssisting(nullptr);
	OrderWalk::AbortOrder(pScene, pWorld, pCrewMembers);
}

bool OrderCarry::AllowsMultipleOrders() noexcept
{
	return false;
}

std::string OrderCarry::GetName() noexcept
{
	return "Carry";
}

bool OrderCarry::ReadyToAbort() noexcept
{
	return OrderWalk::ReadyToAbort();
}

bool OrderCarry::IsIdleOrder() noexcept
{
	return false;
}

bool OrderCarry::CanExecuteIfHurt() noexcept
{
	return false;
}
