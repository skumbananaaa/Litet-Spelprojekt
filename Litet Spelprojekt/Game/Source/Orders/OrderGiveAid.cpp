#include "../../Include/Orders/OrderGiveAid.h"
#include "../../Include/Crew.h"
#include "../../Include/Game.h"

OrderGiveAid::OrderGiveAid(OrderGiveAid* other) : OrderWalk(other)
{
	m_Target = other->m_Target;
	m_InjuredMember = other->m_InjuredMember;
	m_IsAiding = false;
}

OrderGiveAid::OrderGiveAid(Crewmember* injuredMember) : OrderWalk(injuredMember->GetTile() * glm::ivec3(1, 2, 1) - glm::ivec3(1, 0, 0))
{
	m_pAiding = injuredMember;
	m_Target = injuredMember->GetTile();
	m_InjuredMember = injuredMember->GetShipNumber();
	m_IsAiding = false;
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
		if (!m_IsAiding)
		{
			GetCrewMember()->UpdateAnimatedMesh(MESH::ANIMATED_MODEL_WORK);
		}
		res = m_pAiding->Heal(healLevel, dtS);

		if (res)
		{
			Logger::LogEvent(GetCrewMember()->GetName() + " gav hjälp till " + m_pAiding->GetName() + "!", true);
		}
	}
	return res;
}

void OrderGiveAid::OnEnded(Scene * pScene, World * pWorld, Crew * pCrewMembers) noexcept
{
	OrderWalk::OnEnded(pScene, pWorld, pCrewMembers);

	for (uint32 i = 0; i < pCrewMembers->GetCount(); i++)
	{
		Crewmember* member = pCrewMembers->GetMember(i);
		if (member->GetRoom() == GetCrewMember()->GetRoom() && !member->HasRecovered() && !member->IsAbleToWork())
		{
			GiveOrderInbred(new OrderGiveAid(member));
			break;
		}
	}

	GetCrewMember()->ReportPosition();

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

IOrder * OrderGiveAid::Clone() noexcept
{
	return new OrderGiveAid(this);
}

void OrderGiveAid::InitClone(SceneGame * pScene, void * userData) noexcept
{
	m_pAiding = pScene->GetCrew()->GetMember(m_InjuredMember);
	OrderWalk::InitClone(pScene, userData);
}
