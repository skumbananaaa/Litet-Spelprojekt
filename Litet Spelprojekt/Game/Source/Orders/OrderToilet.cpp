#include "../../Include/Orders/OrderToilet.h"
#include "../../Include/Crewmember.h"
#include "../../Include/Scenes/SceneGame.h"
#include <World/World.h>

OrderToilet::OrderToilet(OrderToilet* other) : OrderWalk(other)
{
	m_Name = other->m_Name;
	m_Position = glm::vec3(4.0f);
	m_Timer = 12.0f;
	m_IsAtToilet = false;
}

OrderToilet::OrderToilet(const glm::ivec3& toiletTile, GameObject* pToilet)
	: OrderWalk(toiletTile)
{
	m_Name = pToilet->GetName();
	m_pToilet = pToilet;
	m_Position = glm::vec3(4.0f);
	m_Timer = 12.0f;
	m_IsAtToilet = false;
}

OrderToilet::~OrderToilet()
{
}

void OrderToilet::OnStarted(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept
{
	OrderWalk::OnStarted(pScene, pWorld, pCrewMembers);
}

bool OrderToilet::OnUpdate(Scene* pScene, World* pWorld, Crew* pCrewMembers, float dtS) noexcept
{
	if (!m_IsAtToilet)
	{
		if (OrderWalk::OnUpdate(pScene, pWorld, pCrewMembers, dtS))
		{
			m_IsAtToilet = true;
			Crewmember* pCrewmember = GetCrewMember();
			m_Position = pCrewmember->GetPosition();

			pCrewmember->UpdateAnimatedMesh(MESH::ANIMATED_MODEL_SITTING);

			float yaw = m_pToilet->GetRotation().w;
			while (yaw > glm::two_pi<float>())
			{
				yaw -= glm::two_pi<float>();
			}
			while (yaw < 0)
			{
				yaw += glm::two_pi<float>();
			}
			yaw = fmod(yaw + glm::quarter_pi<float>(), glm::two_pi<float>());
			int rot = (int32)(yaw / glm::half_pi<float>());
			
			if (rot == 0)
			{
				pCrewmember->SetPosition(m_Position + glm::vec3(-0.1f, 0.0f, 0.0f));
				pCrewmember->SetRotation(glm::vec4(0.0f, 1.0f, 0.0f, glm::radians<float>(270.0f)));
			}
			else if (rot == 1)
			{
				pCrewmember->SetPosition(m_Position + glm::vec3(0.0f, 0.0f, 0.1f));
				pCrewmember->SetRotation(glm::vec4(0.0f, 1.0f, 0.0f, glm::radians<float>(0.0f)));
			}
			else if (rot == 2)
			{
				pCrewmember->SetPosition(m_Position + glm::vec3(0.1f, 0.0f, 0.0f));
				pCrewmember->SetRotation(glm::vec4(0.0f, 1.0f, 0.0f, glm::radians<float>(90.0f)));
			}
			else if (rot == 3)
			{
				pCrewmember->SetPosition(m_Position + glm::vec3(0.0f, 0.0f, -0.1f));
				pCrewmember->SetRotation(glm::vec4(0.0f, 1.0f, 0.0f, glm::radians<float>(180.0f)));
			}
		}
	}
	else
	{
		m_Timer -= dtS;
		return m_Timer <= 0.0f;
	}
	glm::ivec3 goal = OrderWalk::m_GoalTile;

	if (pWorld->GetRoom(pWorld->GetLevel(goal.y * 2).GetLevel()[goal.x][goal.z]).IsFireDetected())
	{
		return true;
	}

	return false;
}

void OrderToilet::OnEnded(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept
{
	if (m_IsAtToilet)
	{
		Crewmember* pCrewmember = GetCrewMember();
		pCrewmember->UpdateAnimatedMesh(MESH::ANIMATED_MODEL_IDLE);
		pCrewmember->SetPosition(m_Position);
		pCrewmember->SetRotation(glm::vec4(1.0f, 0.0f, 0.0f, glm::radians<float>(0.0f)));
	}
	else
	{
		OrderWalk::OnEnded(pScene, pWorld, pCrewMembers);
	}
}

bool OrderToilet::CanBeStackedWithSameType() noexcept
{
	return false;
}

std::string OrderToilet::GetName() noexcept
{
	return "Order Toilet";
}

bool OrderToilet::IsIdleOrder() noexcept
{
	return true;
}

IOrder * OrderToilet::Clone() noexcept
{
	return new OrderToilet(this);
}

void OrderToilet::InitClone(SceneGame * pScene, void * userData) noexcept
{
	m_pToilet = pScene->GetGameObject(m_Name);
	OrderWalk::InitClone(pScene, userData);
}
