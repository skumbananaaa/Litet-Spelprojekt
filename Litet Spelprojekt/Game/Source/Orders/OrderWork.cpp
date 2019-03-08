#include "../../Include/Orders/OrderWork.h"
#include "../../Include/Crewmember.h"

OrderWork::OrderWork(const glm::ivec3& instrumentTile, GameObject* pInstrument)
	: OrderWalk(instrumentTile)
{
	m_pInstrument = pInstrument;
	m_Position = glm::vec3(0.0f);
	m_Timer = 15.0f;
	m_IsAtInstrument = false;
}

OrderWork::~OrderWork()
{
}

void OrderWork::OnStarted(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept
{
	OrderWalk::OnStarted(pScene, pWorld, pCrewMembers);
}

bool OrderWork::OnUpdate(Scene* pScene, World* pWorld, Crew* pCrewMembers, float dtS) noexcept
{
	if (!m_IsAtInstrument)
	{
		if (OrderWalk::OnUpdate(pScene, pWorld, pCrewMembers, dtS))
		{
			m_IsAtInstrument = true;
			m_Position = GetCrewMember()->GetPosition();

			GetCrewMember()->UpdateAnimatedMesh(MESH::ANIMATED_MODEL_SLEEP);
			//GetCrewMember()->SetPosition(m_Position + glm::vec3(0.0f, 0.65f, -0.4f));
			//GetCrewMember()->SetRotation(glm::vec4(0.0f, 1.0f, 0.0f, glm::radians<float>(0.0f)));
		}
	}
	else
	{
		m_Timer -= dtS;
		return m_Timer <= 0.0f;
	}

	return false;
}

void OrderWork::OnEnded(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept
{
	GetCrewMember()->UpdateAnimatedMesh(MESH::ANIMATED_MODEL_IDLE);
	//GetCrewMember()->SetPosition(m_Position);
	//GetCrewMember()->SetRotation(glm::vec4(1.0f, 0.0f, 0.0f, glm::radians<float>(0.0f)));

	OrderWalk::OnEnded(pScene, pWorld, pCrewMembers);
}

bool OrderWork::CanBeStackedWithSameType() noexcept
{
	return false;
}

std::string OrderWork::GetName() noexcept
{
	return "Order Toilet";
}

bool OrderWork::ReadyToAbort() noexcept
{
	return OrderWalk::ReadyToAbort();
}

bool OrderWork::IsIdleOrder() noexcept
{
	return true;
}