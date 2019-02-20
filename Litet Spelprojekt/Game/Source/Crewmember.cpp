#include "..\Include\Crewmember.h"
#include "..\Include\Game.h"
#include <System/Random.h>

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


	//Test
	m_HasInjuryBoneBroken = Random::GenerateBool();
	m_HasInjuryBurned = Random::GenerateBool();
	m_HasInjurySmoke = Random::GenerateBool();
	m_SkillFire = Random::GenerateInt(1, 3);
	m_SkillMedic = Random::GenerateInt(1, 3);
	m_SkillStrength = Random::GenerateInt(1, 3);
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
	DeleteSafe(m_pPathFinder);
	DeleteSafe(m_pLight);
	DeleteSafe(m_pTorch);
}

void Crewmember::RunParallel()
{
	//if (!m_pPathFinder->IsGoalSet() && m_NrOfPathTiles == 0) {
		m_pPath = m_pPathFinder->FindPath(m_PlayerTile, m_GoalTile);
		m_NrOfPathTiles = m_pPathFinder->GetNrOfPathTiles();
	//}
}

void Crewmember::Update(float deltaTime) noexcept
{
	FollowPath(deltaTime);
	
	GameObject::Update(deltaTime);
	
	m_pLight->SetPosition(GetPosition());
	m_pTorch->SetPosition(GetPosition());
	m_pTorch->SetDirection(glm::vec3(m_Direction.x, -0.5, m_Direction.z));
}

void Crewmember::OnPicked()
{
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

void Crewmember::OnHovered()
{
	m_IsHovered = true;
	Game::GetGame()->GetUICrewMember()->SetCrewMember(this);
}

void Crewmember::OnNotHovered()
{
	m_IsHovered = false;
	Game::GetGame()->GetUICrewMember()->SetCrewMember(nullptr);
}

void Crewmember::UpdateLastKnownPosition() noexcept
{
	m_LastKnownPosition = GetPosition();
}

const glm::vec3& Crewmember::GetLastKnownPosition() noexcept
{
	return m_LastKnownPosition;
}

int32 Crewmember::TestAgainstRay(const glm::vec3 ray, const glm::vec3 origin, float extension) noexcept
{
	glm::vec3 centre = GetPosition();
	centre.x += extension * glm::floor(centre.y / 2.0f);

	glm::vec3 normals[]{
		m_Direction,
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::normalize(glm::cross(normals[0], normals[1]))
	};

	float h[] = {
		0.1,
		0.9,
		0.25
	};

	float d1[] = {
		glm::dot(centre - normals[0] * h[0], normals[0]),
		glm::dot(centre - normals[1] * h[1], normals[1]),
		glm::dot(centre - normals[2] * h[2], normals[2])
	};
	float d2[] = {
		glm::dot(centre + normals[0] * h[0], normals[0]),
		glm::dot(centre + normals[1] * h[1], normals[1]),
		glm::dot(centre + normals[2] * h[2], normals[2])
	};

	float t1[3];
	float t2[3];
	float t_min[3];
	float t_max[3];

	float t = -1;
	float min_t, max_t;

	for (int j = 0; j < 3; j++)
	{
		if (std::abs(glm::dot(normals[j], ray)) > 0.01)
		{
			t1[j] = (d1[j] - glm::dot(normals[j], origin)) / glm::dot(normals[j], ray);
			t2[j] = (d2[j] - glm::dot(normals[j], origin)) / glm::dot(normals[j], ray);

			t_min[j] = std::min(t1[j], t2[j]);
			t_max[j] = std::max(t1[j], t2[j]);
		}
		else if (-glm::dot(normals[0], centre - origin) - h[j] > 0 || -glm::dot(normals[0], centre - origin) + h[j] < 0)
			return -1;
	}

	min_t = std::max(t_min[0], t_min[1]);
	min_t = std::max(min_t, t_min[2]);
	max_t = std::min(t_max[0], t_max[1]);
	max_t = std::min(max_t, t_max[2]);

	if (min_t <= max_t && max_t >= 0)
	{
		if (t_min > 0)
			t = min_t;
		else
			t = max_t;
	}

	return t;
}

int32 Crewmember::GetShipNumber() const noexcept
{
	return m_ShipNumber;
}

bool Crewmember::IsHovered() const noexcept
{
	return m_IsHovered;
}

int8 Crewmember::GetSkillFire() const noexcept
{
	return m_SkillFire;
}

int8 Crewmember::GetSkillMedic() const noexcept
{
	return m_SkillMedic;
}

int8 Crewmember::GetSkillStrength() const noexcept
{
	return m_SkillStrength;
}

bool Crewmember::HasInjuryBoneBroken() const noexcept
{
	return m_HasInjuryBoneBroken;
}

bool Crewmember::HasInjuryBurned() const noexcept
{
	return m_HasInjuryBurned;
}

bool Crewmember::HasInjurySmoke() const noexcept
{
	return m_HasInjurySmoke;
}

void Crewmember::SetShipNumber(int32 shipnumber) noexcept
{
	m_ShipNumber = shipnumber;
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
	m_PlayerTile = glm::ivec3(std::round(position.x), std::round((position.y - 0.9) / 2),std::round(position.z));
	if (m_NrOfPathTiles <= 0)
	{
		m_TargetTile = m_PlayerTile;
	}
	m_TargetPos = glm::vec3(m_TargetTile.x, m_TargetTile.y * 2 + 0.9, m_TargetTile.z);
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

glm::ivec3 Crewmember::GetTile() const noexcept
{
	return m_PlayerTile;
}

void Crewmember::FindPath(const glm::ivec3& goalPos)
{
	m_GoalTile = glm::ivec3(goalPos.x, goalPos.y / 2, goalPos.z);
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
	if ((std::abs(GetPosition().x - m_TargetPos.x) > 0.01 || std::abs(GetPosition().y - m_TargetPos.y) > 0.01 || std::abs(GetPosition().z - m_TargetPos.z) > 0.01))
	{
		if (GetPosition().x > (std::round(GetPosition().y - 0.9) / 2.0f + 1.0f) * 10.0f + 0.5f)
		{
			GameObject::SetPosition(GetPosition() - glm::vec3(10.0f, 0.0f, 0.0f));
		}
		else if (GetPosition().x <= ((std::round(GetPosition().y - 0.9) / 2.0f) * 10 + 0.5f) * IsExtended() * 0)
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
		m_PlayerTile = glm::ivec3(std::round(GetPosition().x), std::round((GetPosition().y - 0.9) / 2), std::round(GetPosition().z));
		if (m_PlayerTile.x >= 0 && m_PlayerTile.x <= 11)
		{
			SetRoom(m_pPathFinder->GetWorld()->GetLevel(m_PlayerTile.y * 2)->GetLevel()[m_PlayerTile.x][m_PlayerTile.z]);
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