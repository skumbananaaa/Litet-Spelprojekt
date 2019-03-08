#include "../../Include/Orders/OrderWork.h"
#include "../../Include/Crewmember.h"

OrderWork::OrderWork(const glm::ivec3& instrumentTile, GameObject* pInstrument)
	: OrderWalk(instrumentTile)
{
	m_pInstrument = pInstrument;
	m_Position = glm::vec3(4.0f);
	m_Timer = 15.0f;
	m_IsAtInstrument = false;
}

OrderWork::~OrderWork()
{
}

void OrderWork::StartOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept
{
	OrderWalk::StartOrder(pScene, pWorld, pCrewMembers);
}

bool OrderWork::UpdateOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers, float dtS) noexcept
{
	if (!m_IsAtInstrument)
	{
		if (OrderWalk::UpdateOrder(pScene, pWorld, pCrewMembers, dtS))
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

void OrderWork::EndOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept
{
	GetCrewMember()->UpdateAnimatedMesh(MESH::ANIMATED_MODEL_RUN);
	//GetCrewMember()->SetPosition(m_Position);
	//GetCrewMember()->SetRotation(glm::vec4(1.0f, 0.0f, 0.0f, glm::radians<float>(0.0f)));

	OrderWalk::EndOrder(pScene, pWorld, pCrewMembers);
}

void OrderWork::AbortOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept
{
	GetCrewMember()->UpdateAnimatedMesh(MESH::ANIMATED_MODEL_RUN);
	//GetCrewMember()->SetPosition(m_Position);
	//GetCrewMember()->SetRotation(glm::vec4(1.0f, 0.0f, 0.0f, glm::radians<float>(0.0f)));

	OrderWalk::AbortOrder(pScene, pWorld, pCrewMembers);
}

bool OrderWork::AllowsMultipleOrders() noexcept
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