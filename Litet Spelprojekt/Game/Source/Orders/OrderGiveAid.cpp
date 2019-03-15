#include "../../Include/Orders/OrderGiveAid.h"
#include "../../Include/Crew.h"
#include "../../Include/Game.h"

OrderGiveAid::OrderGiveAid(OrderGiveAid* other) : OrderWalk(other)
{
	m_InjuredMember = other->m_InjuredMember;
	m_Target = other->m_Target;
}

OrderGiveAid::OrderGiveAid(Crewmember* injuredMember) : OrderWalk(injuredMember->GetTile())
{
	m_InjuredMember = injuredMember->GetShipNumber();
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
	float healLevel = 0;
	if (GetCrewMember()->GetGroupType() == MEDIC)
	{
		healLevel = 0.2f;
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

	for (uint32 i = 0; i < pCrewMembers->GetCount(); i++)
	{
		Crewmember* member = pCrewMembers->GetMember(i);
		if (member->GetRoom() == GetCrewMember()->GetRoom() && !member->HasRecovered() && !member->IsAbleToWork())
		{
			GiveOrder(new OrderGiveAid(member));
			break;
		}
	}

	GetCrewMember()->ReportPosition();
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

IOrder * OrderGiveAid::Clone() noexcept
{
	return new OrderGiveAid(this);
}

void OrderGiveAid::BeginReplay(SceneGame * pScene, void * userData) noexcept
{
	m_pAiding = pScene->GetCrew()->GetMember(m_InjuredMember);
	OrderWalk::BeginReplay(pScene, userData);
}
