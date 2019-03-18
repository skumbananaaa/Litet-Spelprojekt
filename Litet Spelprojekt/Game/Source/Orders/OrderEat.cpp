#include "../../Include/Orders/OrderEat.h"
#include "../../Include/Crewmember.h"
#include <World/World.h>

OrderEat::OrderEat(const glm::ivec3& chairTile, GameObject* pChair)
	: OrderWalk(chairTile)
{
	m_pChair = pChair;
	m_Position = glm::vec3(4.0f);
	m_Timer = 20.0f;
	m_IsAtChair = false;
}

OrderEat::~OrderEat()
{
}

void OrderEat::OnStarted(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept
{
	OrderWalk::OnStarted(pScene, pWorld, pCrewMembers);
}

bool OrderEat::OnUpdate(Scene* pScene, World* pWorld, Crew* pCrewMembers, float dtS) noexcept
{
	if (!m_IsAtChair)
	{
		if (OrderWalk::OnUpdate(pScene, pWorld, pCrewMembers, dtS))
		{
			m_IsAtChair = true;
			Crewmember* pCrewmember = GetCrewMember();
			m_Position = pCrewmember->GetPosition();

			GetCrewMember()->UpdateAnimatedMesh(MESH::ANIMATED_MODEL_EAT);

			float yaw = m_pChair->GetRotation().w;
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

			const glm::vec3& chairPos = m_pChair->GetPosition();
			if (rot == 0)
			{
				GetCrewMember()->SetPosition(chairPos + glm::vec3(0.0f, 0.1f, -0.05f));
				GetCrewMember()->SetRotation(glm::vec4(0.0f, 1.0f, 0.0f, glm::radians<float>(180.0f)));
			}
			else if (rot == 1)
			{
				GetCrewMember()->SetPosition(chairPos + glm::vec3(-0.05f, 0.1f, 0.0f));
				GetCrewMember()->SetRotation(glm::vec4(0.0f, 1.0f, 0.0f, glm::radians<float>(270.0f)));
			}
			else if (rot == 2)
			{
				GetCrewMember()->SetPosition(chairPos + glm::vec3(0.0f, 0.1f, 0.05f));
				GetCrewMember()->SetRotation(glm::vec4(0.0f, 1.0f, 0.0f, glm::radians<float>(0.0f)));
			}
			else if (rot == 3)
			{
				GetCrewMember()->SetPosition(chairPos + glm::vec3(0.05f, 0.1f, 0.0f));
				GetCrewMember()->SetRotation(glm::vec4(0.0f, 1.0f, 0.0f, glm::radians<float>(90.0f)));
			}
		}
	}
	else
	{
		m_Timer -= dtS;
		return m_Timer <= 0.0f;
	}

	glm::ivec3 goal = OrderWalk::m_GoalTile;
	if (pWorld->GetRoom(pWorld->GetLevel(goal.y).GetLevel()[goal.x][goal.z]).IsFireDetected())
	{
		return true;
	}

	return false;
}

void OrderEat::OnEnded(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept
{
	if (m_IsAtChair)
	{

		Crewmember* pCrewmember = GetCrewMember();
		pCrewmember->SetRotation(glm::vec4(1.0f, 0.0f, 0.0f, glm::radians<float>(0.0f)));
		pCrewmember->SetPosition(m_Position);
	}
	else
	{
		OrderWalk::OnEnded(pScene, pWorld, pCrewMembers);
	}
}

bool OrderEat::CanBeStackedWithSameType() noexcept
{
	return false;
}

std::string OrderEat::GetName() noexcept
{
	return "Order Eat";
}

bool OrderEat::IsIdleOrder() noexcept
{
	return true;
}