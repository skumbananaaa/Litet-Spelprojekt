#include "../../Include/Orders/OrderCarry.h"
#include "../../Include/Crew.h"

OrderCarry::OrderCarry(Crewmember * inNeedOfAssist) : OrderWalk(inNeedOfAssist->GetTile() * glm::ivec3(1, 2, 1))
{
	m_pCarrying = inNeedOfAssist;
}

OrderCarry::~OrderCarry()
{
}

void OrderCarry::OnStarted(Scene * pScene, World * pWorld, Crew * pCrewMembers) noexcept
{
	OrderWalk::OnStarted(pScene, pWorld, pCrewMembers);
	GetCrewMember()->SetAssisting(nullptr);
}

void OrderCarry::OnEnded(Scene * pScene, World * pWorld, Crew * pCrewMembers) noexcept
{
	OrderWalk::OnEnded(pScene, pWorld, pCrewMembers);
	GetCrewMember()->SetAssisting(m_pCarrying);
	GetCrewMember()->GoToSickBay();
	Logger::LogEvent(GetCrewMember()->GetName() + " bar " + m_pCarrying->GetName() + " till sjukstugan!", true);
	GetCrewMember()->ReportPosition();
	//GetCrewMember()->SetAssisting(nullptr);
}

bool OrderCarry::OnUpdate(Scene * pScene, World * pWorld, Crew * pCrewMembers, float dtS) noexcept
{
	if (OrderWalk::OnUpdate(pScene, pWorld, pCrewMembers, dtS))
	{
		return true;
	}
	return false;
}

bool OrderCarry::CanBeStackedWithSameType() noexcept
{
	return false;
}

bool OrderCarry::HasPriority() noexcept
{
	return true;
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

void OrderCarry::RunParallel()
{
	OrderWalk::RunParallel();
}

bool OrderCarry::CanExecuteIfHurt() noexcept
{
	return false;
}
