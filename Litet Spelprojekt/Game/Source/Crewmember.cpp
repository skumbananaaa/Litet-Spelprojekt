#include "..\Include\Crewmember.h"
#include "..\Include\Game.h"
#include <System/Random.h>
#include "..\Include\Game.h"
#include "../Include/Orders/OrderWalk.h"
#include "../Include/Orders/OrderCloseDoor.h"
#include <World/WorldLevel.h>
#include <World/GameObjectDoor.h>

Crewmember::Crewmember(World* world, const glm::vec4& lightColor, const glm::vec3& position, float actionCap, const std::string& name)
	: m_pAssisting(nullptr)
{
	m_ActionCap = actionCap;
	SetName(name);
	m_pWorld = world;
	m_IsPicked = false;
	m_PlayerTile = glm::ivec3(std::round(position.x), std::round((position.y - 0.9) / 2),std::round(position.z));
	SetDirection(glm::vec3(-1.0f, 0.0f, 0.0f));
	SetMaterial(MATERIAL::ANIMATED_MODEL);
	SetAnimatedMesh(MESH::ANIMATED_MODEL);
	SetPosition(position);
	//SetScale(glm::vec3(0.2f));
	UpdateTransform();

	m_LastKnownPosition = position;

	//Test
	m_HasInjuryBoneBroken = Random::GenerateBool();
	m_HasInjuryBurned = Random::GenerateBool();
	m_HasInjurySmoke = Random::GenerateBool();
	m_SkillFire = Random::GenerateInt(1, 3);
	m_SkillMedic = Random::GenerateInt(1, 3);
	m_SkillStrength = Random::GenerateInt(1, 3);

	m_MaxHealth = m_SkillStrength * 100.0f;
	m_Health = m_MaxHealth;
	m_MovementSpeed = CREWMEMBER_FULL_HEALTH_MOVEMENT_SPEED;
}

Crewmember::Crewmember(Crewmember& other)
	: m_pAssisting(nullptr)
{
	m_ActionCap = other.m_ActionCap;
	SetName(other.GetName());
	m_PlayerTile = glm::ivec3(std::round(other.GetPosition().x), std::round((other.GetPosition().y - 0.9) / 2),std::round(other.GetPosition().z));
	SetDirection(other.GetDirection());
	SetMaterial(MATERIAL::CREW_STANDARD);
	SetMesh(MESH::CUBE);
	SetPosition(other.GetPosition());
	SetScale(glm::vec3(0.2, 1.8, 0.5));
	m_MaxHealth = other.m_MaxHealth;
	m_Health = other.m_Health;
	m_MovementSpeed = other.m_MovementSpeed;
	UpdateTransform();
}

Crewmember::~Crewmember()
{

}

void Crewmember::Update(const Camera& camera, float deltaTime) noexcept
{
	SceneGame* pSceneGame = Game::GetGame()->m_pSceneGame;
	m_OrderHandler.Update(pSceneGame, m_pWorld, pSceneGame->GetCrew(), deltaTime);
	GameObject::Update(camera, deltaTime);
	
	UpdateTransform();

	if (isAlive())
	{
		CheckSmokeDamage(m_pWorld->GetLevel(GetPosition().y + 1).GetLevelData(), deltaTime);
		CheckFireDamage(m_pWorld->GetLevel(GetPosition().y).GetLevelData(), deltaTime);
		UpdateHealth(deltaTime);
	}
}

void Crewmember::OnPicked()
{
	uint32 crewRoomIndex = m_pWorld->GetLevel(GetTile().y * 2).GetLevel()[GetTile().x][GetTile().z];
	for (int j = 0; j < m_pWorld->GetDoors().size(); j++)
	{
		glm::ivec3 doorTile = m_pWorld->GetDoor(j);
		uint32 doorRoomIndex = m_pWorld->GetLevel(doorTile.y).GetLevel()[doorTile.x][doorTile.z];
		if (doorRoomIndex == crewRoomIndex)
		{
			GameObjectDoor* door = (GameObjectDoor*)m_pWorld->GetLevel(doorTile.y).GetLevelData()[doorTile.x][doorTile.z].GameObjects[GAMEOBJECT_CONST_INDEX_DOOR];
			if (door->IsOpen())
			{
				m_OrderHandler.GiveOrder(new OrderCloseDoor(door), this);
				break;
			}
		}
	}


	m_IsPicked = true;
}

void Crewmember::OnHovered()
{
	m_IsHovered = true;
	Game::GetGame()->m_pSceneGame->GetUICrewMember()->SetCrewMember(this);
}

void Crewmember::OnNotHovered()
{
	m_IsHovered = false;
	Game::GetGame()->m_pSceneGame->GetUICrewMember()->SetCrewMember(nullptr);
}

void Crewmember::UpdateLastKnownPosition() noexcept
{
	m_LastKnownPosition = GetPosition();
}

const glm::vec3& Crewmember::GetLastKnownPosition() const noexcept
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

bool Crewmember::IsPicked() const noexcept
{
	return m_IsPicked;
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
	return m_HasInjuryBurned > 1.0;
}

bool Crewmember::HasInjurySmoke() const noexcept
{
	return m_HasInjurySmoke > 1.0;
}

bool Crewmember::isAlive() const noexcept
{
	return m_Health > 0.0f;
}

void Crewmember::ApplyBoneInjury()
{
	m_HasInjuryBoneBroken = true;
}

void Crewmember::ApplyBurnInjury(float burn)
{
	m_HasInjuryBurned += burn;
}

void Crewmember::SetAssisting(Crewmember* inNeed)
{
	m_pAssisting = inNeed;
}

bool Crewmember::Heal(int8 skillLevel)
{
	bool res = false;
	if (HasInjuryBoneBroken())
	{
		m_HasInjuryBoneBroken /= skillLevel;
	}
	else if (HasInjuryBurned())
	{
		m_HasInjuryBurned /= skillLevel;
	}
	else if (HasInjurySmoke())
	{
		m_HasInjurySmoke /= skillLevel;
	}
	else
	{
		res = true;
	}
	return res;
}

void Crewmember::SetShipNumber(int32 shipnumber) noexcept
{
	m_ShipNumber = shipnumber;
}

void Crewmember::CheckSmokeDamage(const TileData*const* data, float dt) noexcept
{
	float smokeDmgSpeed = 0.1f;
	TileData tileData = data[m_PlayerTile.x][m_PlayerTile.z];
	if (tileData.SmokeAmount - tileData.SmokeLimit >= 1.0)
	{
		bool isSmoked = HasInjurySmoke();
		m_HasInjurySmoke += (tileData.SmokeAmount / tileData.SmokeLimit) * smokeDmgSpeed * dt;

		if (isSmoked != HasInjurySmoke())
		{
			Logger::LogEvent(GetName() + " got smoked!" + std::to_string(m_HasInjurySmoke));
		}
	}
}

void Crewmember::CheckFireDamage(const TileData * const * data, float dt) noexcept
{
	float burnSpeed = 0.1f;
	TileData tileData = data[m_PlayerTile.x][m_PlayerTile.z];
	if (tileData.Temp >= tileData.BurnsAt)
	{
		bool isBurned = HasInjuryBurned();
		m_HasInjuryBurned += (tileData.Temp / tileData.BurnsAt) * burnSpeed * dt;
		if (isBurned != HasInjuryBurned())
		{
			Logger::LogEvent(GetName() + " got burned!" + std::to_string(m_HasInjuryBurned));
		}
	}
}

void Crewmember::UpdateHealth(float dt)
{
	//Tweak here!
	float smokeDmgSpeed = 1.0f;
	float burnDmgSpeed = 1.0f;

	if (HasInjurySmoke())
	{
		m_Health -= (std::log10(m_HasInjurySmoke)) * smokeDmgSpeed * dt;
	}

	if (HasInjuryBurned())
	{
		m_Health -= (std::log10(m_HasInjuryBurned)) * burnDmgSpeed * dt;
	}

	if (HasInjuryBoneBroken())
	{
		//m_Health -= m_HasInjuryBoneBroken * dt;
	}

	if (m_Health <= 0.0f)
	{
		Logger::LogEvent(GetName() + " has fainted!", false);
		m_MovementSpeed = CREWMEMBER_DEAD_MOVEMENT_SPEED;
	}
	else if (m_Health < m_MaxHealth)
	{
		if (m_Health > 0.5f * m_MaxHealth)
		{
			m_MovementSpeed = CREWMEMBER_LIGHTLY_INJURED_MOVEMENT_SPEED;
		}
		else
		{
			m_MovementSpeed = CREWMEMBER_SERIOUSLY_INJURED_MOVEMENT_SPEED;
		}
	}
	else
	{
		m_MovementSpeed = CREWMEMBER_FULL_HEALTH_MOVEMENT_SPEED;
	}
}

void Crewmember::Move(const glm::vec3& dir, bool allowMult, float dtS)
{
	//Multiply In MovementSpeed Multipliers;
	float movementSpeedMultiplier = 1.0f;

	if (allowMult)
	{
		const glm::ivec3& tilePos = GetTile();
		uint32 yPos = tilePos.y * 2;

		//Water Modifier
		if (m_pWorld->GetLevel(yPos).GetLevelData()[tilePos.x][tilePos.z].WaterLevel > WATER_UPDATE_LEVEL_INTERVAL)
		{
			movementSpeedMultiplier *= glm::max(0.2f, 1.0f - (m_pWorld->GetLevel(yPos).GetLevelData()[tilePos.x][tilePos.z].WaterLevel / WATER_MAX_LEVEL));
		}
	}

	glm::vec3 res = GetPosition() + dir * m_MovementSpeed * movementSpeedMultiplier * dtS;
	SetPosition(res);
}

const float Crewmember::GetActionCapacity() const
{
	return m_ActionCap;
}

void Crewmember::SetPosition(const glm::vec3& position) noexcept
{
	m_PlayerTile = glm::ivec3(std::round(position.x), std::round((position.y - 0.9) / 2),std::round(position.z));

	if (m_PlayerTile.x >= 0 && m_PlayerTile.x <= 11)
	{
		SetRoom(m_pWorld->GetLevel(m_PlayerTile.y * 2).GetLevel()[m_PlayerTile.x][m_PlayerTile.z]);
	}

	GameObject::SetPosition(position);
}

const glm::vec3& Crewmember::GetDirection() const noexcept
{
	return m_Direction;
}

float Crewmember::GetMovementSpeed() const noexcept
{
	return m_MovementSpeed;
}

void Crewmember::SetDirection(const glm::vec3& direction) noexcept
{
	m_Direction = glm::normalize(direction);
	float angle = std::atan2f(1.0f * m_Direction.x, -1.0f * m_Direction.z);
	SetRotation(glm::vec4(0.0f, 1.0f, 0.0f, -angle));
}

glm::ivec3 Crewmember::GetTile() const noexcept
{
	return m_PlayerTile;
}

void Crewmember::FindPath(const glm::ivec3& goalPos)
{
	if (!HasInjurySmoke() && !HasInjuryBurned())
	{
		m_OrderHandler.GiveOrder(new OrderWalk(goalPos), this);
	}
	else
	{
		Logger::LogEvent(GetName() + " cannot move!", true);
	}
}

void Crewmember::LookForDoor(World* pWorld, Scene* pScene)
{
	//StartOrder(pScene, pWorld, this);
}

void Crewmember::CloseDoorOrder(glm::ivec3 doorTile)
{
	FindPath(doorTile);
}

void Crewmember::SetActionCapacity(const float actionCap)
{
	m_ActionCap = actionCap;
}
