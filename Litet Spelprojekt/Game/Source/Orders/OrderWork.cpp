#include "../../Include/Orders/OrderWork.h"
#include "../../Include/Crewmember.h"
#include "../../Include/Scenes/SceneGame.h"
#include <World/World.h>

OrderWork::OrderWork(OrderWork* other) : OrderWalk(other)
{
	m_Name = other->m_Name;
	m_Position = glm::vec3(4.0f);
	m_Timer = 45.0f;
	m_IsAtInstrument = false;
}

OrderWork::OrderWork(const glm::ivec3& instrumentTile, GameObject* pInstrument)
	: OrderWalk(instrumentTile)
{
	m_Name = pInstrument->GetName();
	m_pInstrument = pInstrument;
	m_Position = glm::vec3(4.0f);
	m_Timer = 45.0f;
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
			Crewmember* pCrewmember = GetCrewMember();
			m_Position = pCrewmember->GetPosition();

			pCrewmember->UpdateAnimatedMesh(MESH::ANIMATED_MODEL_WORK);

			float yaw = m_pInstrument->GetRotation().w;
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
				pCrewmember->SetPosition(m_Position + glm::vec3(0.3f, 0.0f, 0.0));
				pCrewmember->SetRotation(glm::vec4(0.0f, 1.0f, 0.0f, glm::radians<float>(90.0f)));
			}
			else if (rot == 1)
			{
				pCrewmember->SetPosition(m_Position + glm::vec3(0.0f, 0.0, -0.3f));
				pCrewmember->SetRotation(glm::vec4(0.0f, 1.0f, 0.0f, glm::radians<float>(180.0f)));
			}
			else if (rot == 2)
			{
				pCrewmember->SetPosition(m_Position + glm::vec3(-0.3f, 0.0f, 0.0));
				pCrewmember->SetRotation(glm::vec4(0.0f, 1.0f, 0.0f, glm::radians<float>(270.0f)));
			}
			else if (rot == 3)
			{
				pCrewmember->SetPosition(m_Position + glm::vec3(0.0f, 0.0, 0.3f));
				pCrewmember->SetRotation(glm::vec4(0.0f, 1.0f, 0.0f, glm::radians<float>(0.0f)));
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

void OrderWork::OnEnded(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept
{
	if (m_IsAtInstrument)
	{
		Crewmember* pCrewmember = GetCrewMember();
		pCrewmember->SetPosition(m_Position);
		pCrewmember->SetRotation(glm::vec4(1.0f, 0.0f, 0.0f, glm::radians<float>(0.0f)));
	}
	else
	{
		OrderWalk::OnEnded(pScene, pWorld, pCrewMembers);
	}
}

bool OrderWork::CanBeStackedWithSameType() noexcept
{
	return false;
}

std::string OrderWork::GetName() noexcept
{
	return "Order Work";
}

bool OrderWork::IsIdleOrder() noexcept
{
	return true;
}

IOrder* OrderWork::Clone() noexcept
{
	return new OrderWork(this);
}

void OrderWork::InitClone(SceneGame * pScene, void * userData) noexcept
{
	m_pInstrument = pScene->GetGameObject(m_Name);
	OrderWalk::InitClone(pScene, userData);
}
