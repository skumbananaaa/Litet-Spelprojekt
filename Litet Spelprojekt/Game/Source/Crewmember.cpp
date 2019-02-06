#include "..\Include\Crewmember.h"

Crewmember::Crewmember(const glm::vec4& lightColor, const glm::vec3& position, float actionCap, const std::string& name) 
	: m_pLight(new PointLight(position, lightColor))
{
	m_ActionCap = actionCap;
	m_Name = name;
	m_NrOfPathTiles = 0;
	m_PlayerTile = glm::ivec3(std::round(position.x), std::round((position.y - 0.9) / 2),std::round(position.z));
	m_TargetTile = m_PlayerTile;
	m_TargetPos = glm::vec3(m_TargetTile.x, m_TargetTile.y * 2 + 0.9, m_TargetTile.z);
	
	this->SetMaterial(MATERIAL::CREW_STANDARD);
	this->SetMesh(MESH::CUBE);
	this->SetPosition(position);
	this->SetScale(glm::vec3(0.2, 1.8, 0.5));
	this->UpdateTransform();
}

Crewmember::Crewmember(Crewmember& other)
	: m_pLight(new PointLight(other.GetPosition(), other.m_pLight->GetColor()))
{
	m_ActionCap = other.m_ActionCap;
	m_Name = other.m_Name;
	m_NrOfPathTiles = 0;
	m_PlayerTile = glm::ivec3(std::round(other.GetPosition().x), std::round((other.GetPosition().y - 0.9) / 2),std::round(other.GetPosition().z));
	m_TargetTile = m_PlayerTile;
	m_TargetPos = glm::vec3(m_TargetTile.x, m_TargetTile.y * 2 + 0.9, m_TargetTile.z);
	this->SetMaterial(MATERIAL::WHITE);
	this->SetMesh(MESH::CUBE);
	this->SetPosition(other.GetPosition());
	this->UpdateTransform();
}

Crewmember::~Crewmember()
{
	Delete(m_pPathFinder);
}

void Crewmember::RunParallel()
{
	if (!m_pPathFinder->IsGoalSet() && m_NrOfPathTiles == 0) {
		m_pPath = m_pPathFinder->FindPath(m_PlayerTile, m_GoalTile);
		m_NrOfPathTiles = m_pPathFinder->GetNrOfPathTiles();
	}
}

void Crewmember::Update(float deltaTime)
{

	FollowPath(deltaTime);
	GameObject::Update(deltaTime);
	m_pLight->SetPosition(GetPosition());
}



void Crewmember::Move(const glm::vec3 & dir)
{
	glm::vec3 res = this->GetPosition() + dir;
	this->SetPosition(res);
}

PointLight * Crewmember::GetLight() const
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
	m_PlayerTile = glm::ivec3(std::round(position.x), std::round((position.y - 0.9) / 2),std::round(position.z));
	m_TargetTile = m_PlayerTile;
	m_TargetPos = glm::vec3(m_TargetTile.x, m_TargetTile.y * 2 + 0.9, m_TargetTile.z);
	GameObject::SetPosition(position);
}

void Crewmember::FindPath(const glm::ivec3& goalPos)
{
	m_GoalTile = goalPos;
	ThreadHandler::RequestExecution(this);
}

void Crewmember::FollowPath(float dtS)
{
	if (m_NrOfPathTiles > 0)
	{
		if (m_PlayerTile == m_TargetTile)
		{
			m_TargetTile = m_pPath[--m_NrOfPathTiles];
			m_TargetPos = glm::vec3(m_TargetTile.x, m_TargetTile.y * 2 + 0.9, m_TargetTile.z);
		}
	}
	if (std::abs(this->GetPosition().x - m_TargetPos.x) > 0.01 || std::abs(this->GetPosition().y - m_TargetPos.y) > 0.01 || std::abs(this->GetPosition().z - m_TargetPos.z) > 0.01)
	{
		glm::vec3 move(m_TargetPos.x - this->GetPosition().x, m_TargetPos.y - this->GetPosition().y, m_TargetPos.z - this->GetPosition().z);
		move = glm::normalize(move);
		if (std::abs(move.y) > 0.01)
		{
			move.y /= std::abs(move.y);
			GameObject::SetPosition(this->GetPosition() + glm::vec3(0, move.y * dtS, 0));
		}
		else
		{
			GameObject::SetPosition(this->GetPosition() + glm::vec3(move.x * dtS, 0, move.z * dtS));
		}
		m_PlayerTile = glm::ivec3(std::round(this->GetPosition().x), std::round((this->GetPosition().y - 0.9) / 2), std::round(this->GetPosition().z));
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