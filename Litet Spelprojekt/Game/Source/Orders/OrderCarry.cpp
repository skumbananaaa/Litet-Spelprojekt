#include "../../Include/Orders/OrderCarry.h"
#include "../../Include/Crew.h"
#include "../../Include/Game.h"

OrderCarry::OrderCarry(OrderCarry* other) : OrderWalk(other)
{
	m_InNeedOfAssist = other->m_InNeedOfAssist;
}

OrderCarry::OrderCarry(Crewmember* inNeedOfAssist) : OrderWalk(inNeedOfAssist->GetTile() * glm::ivec3(1, 2, 1))
{
	m_pCarrying = inNeedOfAssist;
	m_InNeedOfAssist = inNeedOfAssist->GetShipNumber();
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
	
	GetCrewMember()->ReportPosition();
}

bool OrderCarry::OnUpdate(Scene * pScene, World * pWorld, Crew * pCrewMembers, float dtS) noexcept
{
	if (OrderWalk::OnUpdate(pScene, pWorld, pCrewMembers, dtS))
	{
		Crewmember* pCrewmember = GetCrewMember();
		pCrewmember->SetAssisting(m_pCarrying);
		pCrewmember->GoToSickBay();
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

IOrder * OrderCarry::Clone() noexcept
{
	return new OrderCarry(this);
}

void OrderCarry::InitClone(SceneGame* pScene, void* userData) noexcept
{
	m_pCarrying = pScene->GetCrew()->GetMember(m_InNeedOfAssist);
	OrderWalk::InitClone(pScene, userData);
}
