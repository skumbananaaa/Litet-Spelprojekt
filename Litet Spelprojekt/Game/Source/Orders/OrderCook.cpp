#include "../../Include/Orders/OrderCook.h"
#include "../../Include/Crewmember.h"

OrderCook::OrderCook(const glm::ivec3& ovenTile, GameObject* pOven)
	: OrderWalk(ovenTile)
{
	m_pOven = pOven;
	m_Position = glm::vec3(4.0f);
	m_Timer = 15.0f;
	m_IsAtOven = false;
}

OrderCook::~OrderCook()
{
}

void OrderCook::OnStarted(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept
{
	OrderWalk::OnStarted(pScene, pWorld, pCrewMembers);
}

bool OrderCook::OnUpdate(Scene* pScene, World* pWorld, Crew* pCrewMembers, float dtS) noexcept
{
	if (!m_IsAtOven)
	{
		if (OrderWalk::OnUpdate(pScene, pWorld, pCrewMembers, dtS))
		{
			m_IsAtOven = true;
			m_Position = GetCrewMember()->GetPosition();

			GetCrewMember()->UpdateAnimatedMesh(MESH::ANIMATED_MODEL_WORK);

			float yaw = m_pOven->GetRotation().w;
			while (yaw > glm::two_pi<float>())
			{
				yaw -= glm::two_pi<float>();
			}
			while (yaw < 0)
			{
				yaw += glm::two_pi<float>();
			}
			yaw = fmod(yaw + glm::quarter_pi<float>(), glm::two_pi<float>());
			int rot = yaw / glm::half_pi<float>();

			const glm::vec3& chairPos = m_pOven->GetPosition();
			if (rot == 0)
			{
				GetCrewMember()->SetPosition(chairPos + glm::vec3(0.0f, 0.1f, -0.9f));
				GetCrewMember()->SetRotation(glm::vec4(0.0f, 1.0f, 0.0f, glm::radians<float>(180.0f)));
			}
			else if (rot == 1)
			{
				GetCrewMember()->SetPosition(chairPos + glm::vec3(-0.9f, 0.1f, 0.0f));
				GetCrewMember()->SetRotation(glm::vec4(0.0f, 1.0f, 0.0f, glm::radians<float>(270.0f)));
			}
			else if (rot == 2)
			{
				GetCrewMember()->SetPosition(chairPos + glm::vec3(0.0f, 0.1f, 0.9f));
				GetCrewMember()->SetRotation(glm::vec4(0.0f, 1.0f, 0.0f, glm::radians<float>(0.0f)));
			}
			else if (rot == 3)
			{
				GetCrewMember()->SetPosition(chairPos + glm::vec3(0.9f, 0.1f, 0.0f));
				GetCrewMember()->SetRotation(glm::vec4(0.0f, 1.0f, 0.0f, glm::radians<float>(90.0f)));
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

void OrderCook::OnEnded(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept
{
	if (m_IsAtOven)
	{
		GetCrewMember()->UpdateAnimatedMesh(MESH::ANIMATED_MODEL_RUN);
		GetCrewMember()->SetRotation(glm::vec4(1.0f, 0.0f, 0.0f, glm::radians<float>(0.0f)));
		GetCrewMember()->SetPosition(m_Position);
	}
	else
	{
		OrderWalk::OnEnded(pScene, pWorld, pCrewMembers);
	}
}

bool OrderCook::CanBeStackedWithSameType() noexcept
{
	return false;
}

std::string OrderCook::GetName() noexcept
{
	return "Order Cook";
}

bool OrderCook::IsIdleOrder() noexcept
{
	return true;
}