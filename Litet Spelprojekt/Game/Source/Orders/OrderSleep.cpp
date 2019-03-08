#include "../../Include/Orders/OrderSleep.h"
#include "../../Include/Crewmember.h"
#include <System/Random.h>
#include <IO/ResourceHandler.h>

OrderSleep::OrderSleep(const glm::ivec3& bedTile, GameObject* pBed)
	: OrderWalk(bedTile)
{
	m_pBed = pBed;
	m_Position = glm::vec3(0.0f);
	m_Timer = 60.0f;
	m_IsAtBed = false;
}

OrderSleep::~OrderSleep()
{
}

void OrderSleep::OnStarted(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept
{
	OrderWalk::OnStarted(pScene, pWorld, pCrewMembers);
}

bool OrderSleep::OnUpdate(Scene* pScene, World* pWorld, Crew* pCrewMembers, float dtS) noexcept
{
	if (!m_IsAtBed)
	{
		if (OrderWalk::OnUpdate(pScene, pWorld, pCrewMembers, dtS))
		{
			m_IsAtBed = true;
			m_Position = GetCrewMember()->GetPosition();
			
			GetCrewMember()->UpdateAnimatedMesh(MESH::ANIMATED_MODEL_SLEEP);

			float yaw = m_pBed->GetRotation().w;
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

			bool up = Random::GenerateBool();
			float yOffset = up ? 0.5f : -0.35f;
			if (rot == 0)
			{
				GetCrewMember()->SetPosition(m_Position + glm::vec3(0.0f, yOffset, -0.3f));
				GetCrewMember()->SetRotation(glm::vec4(0.0f, 1.0f, 0.0f, glm::radians<float>(0.0f)));
			}
			else if (rot == 1)
			{
				GetCrewMember()->SetPosition(m_Position + glm::vec3(-0.3f, yOffset, 0.0f));
				GetCrewMember()->SetRotation(glm::vec4(0.0f, 1.0f, 0.0f, glm::radians<float>(90.0f)));
			}
			else if (rot == 2)
			{
				GetCrewMember()->SetPosition(m_Position + glm::vec3(0.0f, yOffset, 0.3f));
				GetCrewMember()->SetRotation(glm::vec4(0.0f, 1.0f, 0.0f, glm::radians<float>(180.0f)));
			}
			else if (rot == 3)
			{
				GetCrewMember()->SetPosition(m_Position + glm::vec3(0.3f, yOffset, 0.0f));
				GetCrewMember()->SetRotation(glm::vec4(0.0f, 1.0f, 0.0f, glm::radians<float>(270.0f)));
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

void OrderSleep::OnEnded(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept
{
	GetCrewMember()->UpdateAnimatedMesh(MESH::ANIMATED_MODEL_IDLE);
	GetCrewMember()->SetPosition(m_Position);
	GetCrewMember()->SetRotation(glm::vec4(1.0f, 0.0f, 0.0f, glm::radians<float>(0.0f)));

	OrderWalk::OnEnded(pScene, pWorld, pCrewMembers);
}

bool OrderSleep::CanBeStackedWithSameType() noexcept
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