#include "../../Include/Orders/OrderSleep.h"
#include "../../Include/Crewmember.h"

OrderSleep::OrderSleep(const glm::ivec3& bedTile, GameObject* pBed)
	: OrderWalk(bedTile)
{
	m_pBed = pBed;
	m_Position = glm::vec3(0.0f);
	m_Timer = 5.0f;
	m_IsAtBed = false;
}

OrderSleep::~OrderSleep()
{
}

void OrderSleep::StartOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept
{
	OrderWalk::StartOrder(pScene, pWorld, pCrewMembers);
}

bool OrderSleep::UpdateOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers, float dtS) noexcept
{
	if (!m_IsAtBed)
	{
		if (OrderWalk::UpdateOrder(pScene, pWorld, pCrewMembers, dtS))
		{
			m_IsAtBed = true;
			m_Position = GetCrewMember()->GetPosition();
			
			glm::vec4 rotation = m_pBed->GetRotation();
			if (rotation.w > 0.0f)
			{
				GetCrewMember()->SetPosition(m_Position + glm::vec3(0.0f, 0.65f, -1.35f));
				GetCrewMember()->SetRotation(glm::vec4(1.0f, 0.0f, 0.0f, glm::radians<float>(90.0f)));
			}
			else
			{
				GetCrewMember()->SetPosition(m_Position + glm::vec3(-1.35f, 0.65f, 0.0f));
				GetCrewMember()->SetRotation(glm::vec4(glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f)), glm::radians<float>(90.0f)));
			}
		}
	}
	else 
	{	
		m_Timer -= dtS;
		return m_Timer <= 0.0f;
	}

	return false;
}

void OrderSleep::EndOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept
{
	GetCrewMember()->SetPosition(m_Position);
	GetCrewMember()->SetRotation(glm::vec4(1.0f, 0.0f, 0.0f, glm::radians<float>(0.0f)));

	OrderWalk::EndOrder(pScene, pWorld, pCrewMembers);
}

void OrderSleep::AbortOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept
{
	GetCrewMember()->SetPosition(m_Position);
	GetCrewMember()->SetRotation(glm::vec4(1.0f, 0.0f, 0.0f, glm::radians<float>(0.0f)));

	OrderWalk::AbortOrder(pScene, pWorld, pCrewMembers);
}

bool OrderSleep::AllowsMultipleOrders() noexcept
{
	return false;
}

std::string OrderSleep::GetName() noexcept
{
	return "Order Sleep";
}

bool OrderSleep::ReadyToAbort() noexcept
{
	return OrderWalk::ReadyToAbort();
}

bool OrderSleep::IsIdleOrder() noexcept
{
	return true;
}

void OrderSleep::RunParallel()
{
	OrderWalk::RunParallel();
}
