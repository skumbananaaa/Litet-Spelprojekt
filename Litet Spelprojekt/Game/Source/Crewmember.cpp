#include "..\Include\Crewmember.h"
#include "..\Include\Game.h"

Crewmember::Crewmember(const glm::vec4& lightColor, const glm::vec3& position, float actionCap, const std::string& name) 
	: m_pLight(new PointLight(position, lightColor))
{
	m_ActionCap = actionCap;
	SetName(name);
	m_NrOfPathTiles = 0;
	m_PlayerTile = glm::ivec3(std::round(position.x), std::round((position.y - 0.9) / 2),std::round(position.z));
	m_TargetTile = m_PlayerTile;
	m_TargetPos = glm::vec3(m_TargetTile.x, m_TargetTile.y * 2 + 0.9, m_TargetTile.z);
	SetDirection(glm::vec3(1.0f, 0.0f, 0.0f));
	m_pTorch = new SpotLight(position, glm::cos(glm::radians(15.0f)), glm::cos(glm::radians(25.0f)), glm::vec3(m_Direction.x, 0.0, m_Direction.z), glm::vec4(0.0f));
	SetMaterial(MATERIAL::CREW_STANDARD);
	SetMesh(MESH::CUBE);
	SetPosition(position);
	SetScale(glm::vec3(0.2, 1.8, 0.5));
	UpdateTransform();
}

Crewmember::Crewmember(Crewmember& other)
	: m_pLight(new PointLight(other.GetPosition(), other.m_pLight->GetColor()))
{
	m_ActionCap = other.m_ActionCap;
	SetName(other.GetName());
	m_NrOfPathTiles = 0;
	m_PlayerTile = glm::ivec3(std::round(other.GetPosition().x), std::round((other.GetPosition().y - 0.9) / 2),std::round(other.GetPosition().z));
	m_TargetTile = m_PlayerTile;
	m_TargetPos = glm::vec3(m_TargetTile.x, m_TargetTile.y * 2 + 0.9, m_TargetTile.z);
	SetDirection(other.GetDirection());
	m_pTorch = new SpotLight(other.GetPosition(), glm::cos(glm::radians(15.0f)), glm::cos(glm::radians(25.0f)), glm::vec3(m_Direction.x, 0.0, m_Direction.z), other.m_pTorch->GetColor());
	SetMaterial(MATERIAL::CREW_STANDARD);
	SetMesh(MESH::CUBE);
	SetPosition(other.GetPosition());
	SetScale(glm::vec3(0.2, 1.8, 0.5));
	UpdateTransform();
}

Crewmember::~Crewmember()
{
	Delete(m_pPathFinder);
}

void Crewmember::RunParallel()
{
	//if (!m_pPathFinder->IsGoalSet() && m_NrOfPathTiles == 0) {
		m_pPath = m_pPathFinder->FindPath(m_PlayerTile, m_GoalTile);
		m_NrOfPathTiles = m_pPathFinder->GetNrOfPathTiles();
	//}
}

void Crewmember::Update(float deltaTime)
{
	FollowPath(deltaTime);
	GameObject::Update(deltaTime);
	m_pLight->SetPosition(GetPosition());
	m_pTorch->SetPosition(GetPosition());
	m_pTorch->SetDirection(glm::vec3(m_Direction.x, -0.5, m_Direction.z));
}

void Crewmember::OnPicked()
{
	Game::GetGame()->m_pUICrewMember->SetCrewMember(this);

	if (m_pLight->GetColor() == CHOSEN_LIGHT)
	{
		m_pLight->SetColor(DEFAULT_LIGHT);
	}
	else if (m_pTorch->GetColor() == CHOSEN_LIGHT)
	{
		m_pTorch->SetColor(DEFAULT_LIGHT);
	}
	else if (m_pLight->GetColor() == DEFAULT_LIGHT)
	{
		m_pLight->SetColor(CHOSEN_LIGHT);
	}
	else if (m_pTorch->GetColor() == DEFAULT_LIGHT)
	{
		m_pTorch->SetColor(CHOSEN_LIGHT);
	}
}

void Crewmember::Move(const glm::vec3 & dir)
{
	glm::vec3 res = GetPosition() + dir;
	SetPosition(res);
}

SpotLight* Crewmember::GetTorch() const
{
	return m_pTorch;
}

PointLight* Crewmember::GetLight() const
{
	return m_pLight;
}

const float Crewmember::GetActionCapacity() const
{
	return m_ActionCap;
}

const bool Crewmember::IsMoving() const
{
	return (bool)m_NrOfPathTiles || m_pPathFinder->IsGoalSet();
}

void Crewmember::SetPosition(const glm::vec3 & position) noexcept
{
	m_PlayerTile = glm::ivec3(std::round(position.x) - std::round(position.y - 0.9) * 5 * IsExtended(), std::round((position.y - 0.9) / 2),std::round(position.z));
	if (m_NrOfPathTiles <= 0)
	{
		m_TargetTile = m_PlayerTile;
	}
	m_TargetPos = glm::vec3(m_TargetTile.x + m_TargetTile.y * 10 * IsExtended(), m_TargetTile.y * 2 + 0.9, m_TargetTile.z);
	GameObject::SetPosition(position);
}

const glm::vec3& Crewmember::GetDirection() const noexcept
{
	return m_Direction;
}

void Crewmember::SetDirection(const glm::vec3 & direction) noexcept
{
	m_Direction = glm::normalize(direction);
	float angle = std::atan2f(1.0f * m_Direction.z - 0.0f * m_Direction.x, 1.0f * m_Direction.x + 0.0f * m_Direction.z);
	SetRotation(glm::vec4(0.0f, 1.0f, 0.0f, -angle));
}

void Crewmember::SwitchLight() noexcept
{
	glm::vec4 temp = m_pTorch->GetColor();
	m_pTorch->SetColor(m_pLight->GetColor());
	m_pLight->SetColor(temp);
}

void Crewmember::FindPath(const glm::ivec3& goalPos)
{
	if (!IsExtending())
	{
		m_GoalTile = glm::ivec3(goalPos.x - goalPos.y * 5 * IsExtended(), goalPos.y / 2, goalPos.z);
		ThreadHandler::RequestExecution(this);
	}
}

void Crewmember::FollowPath(float dtS)
{
	if (!IsExtending())
	{
		if (m_NrOfPathTiles > 0)
		{
			if (m_PlayerTile == m_TargetTile)
			{
				m_TargetTile = m_pPath[--m_NrOfPathTiles];
				m_TargetPos = glm::vec3(m_TargetTile.x + m_TargetTile.y * 10 * IsExtended(), m_TargetTile.y * 2 + 0.9, m_TargetTile.z);
				/*if (m_PlayerTile.y != m_TargetTile.y)
				{
					GameObject::SetPosition(GetPosition() + glm::vec3((m_TargetTile.y - m_PlayerTile.y) * 10 * IsExtended(), 0, 0));
				}*/
			}
		}
		if ((std::abs(GetPosition().x - m_TargetPos.x) > 0.01 || std::abs(GetPosition().y - m_TargetPos.y) > 0.01 || std::abs(GetPosition().z - m_TargetPos.z) > 0.01))
		{
			if (GetPosition().x > (std::round(GetPosition().y - 0.9) / 2.0f + 1.0f) * 10.0f)
			{
				GameObject::SetPosition(GetPosition() - glm::vec3(10.0f, 0.0f, 0.0f));
			}
			else if (GetPosition().x <= (std::round(GetPosition().y - 0.9) / 2.0f) * 10 * IsExtended())
			{
				GameObject::SetPosition(GetPosition() + glm::vec3(10.0f, 0.0f, 0.0f));
			}
			glm::vec3 move = m_TargetPos - GetPosition();
			move = glm::normalize(move);
			if (std::abs(move.y) > 0.01)
			{
				move.y /= std::abs(move.y);
				SetDirection(glm::vec3(0, 0, 1));
				GameObject::SetPosition(GetPosition() + glm::vec3(0, move.y * dtS, 0));
			}
			else
			{
				SetDirection(glm::vec3(move.x, 0, move.z));
				GameObject::SetPosition(GetPosition() + m_Direction * dtS);
			}
			m_PlayerTile = glm::ivec3(std::round(GetPosition().x) - std::round(GetPosition().y - 0.9) * 5 * IsExtended(), std::round((GetPosition().y - 0.9) / 2), std::round(GetPosition().z));
		}
	}
}

void Crewmember::SetActionCapacity(const float actionCap)
{
	m_ActionCap = actionCap;
}

void Crewmember::SetPath(const World* world)
{
	m_pPathFinder = new Path(world);
}