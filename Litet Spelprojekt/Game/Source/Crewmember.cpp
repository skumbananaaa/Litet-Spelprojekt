#include "..\Include\Crewmember.h"
#include "..\Include\Game.h"
#include <System/Random.h>
#include "../Include/Orders/OrderWalk.h"
#include "../Include/Orders/OrderCloseDoor.h"
#include "../Include/Orders/OrderWalkMedicBay.h"
#include <World/WorldLevel.h>
#include "../Include/GameObjectDoor.h"
#include "../Include/Orders/OrderSchedule.h"
#include "../Include/Orders/OrderCarry.h"
#include "../Include/GameState.h"
#include "../Include/Scenes/SceneGame.h"
#include "../Include/ReplayHandler.h"
#include "../Include/Orders/ReplayablePosition.h"

Crewmember::Crewmember(World* world, const glm::vec3& position, const std::string& name, GroupType groupType)
	: m_pAssisting(nullptr),
	m_pAudioSourceScream(nullptr),
	m_OrderHandler(this),
	m_pUISelectedCrew(nullptr),
	m_GearIsEquipped(false),
	m_HasEquippedExtinguisher(false),
	m_IsCarried(false),
	m_HasTriedToWalkToSickbay(false),
	m_pShadow(nullptr),
	m_ReplayPositionSyncTimer(0.0f)
{
	//Set crewmembers to be updated
	m_IsTickable = true;

	SetName(name);
	SetWorld(world);
	m_IsPicked = false;
	m_PlayerTile = glm::ivec3(std::round(position.x), std::round((position.y) / 2),std::round(position.z));
	SetDirection(glm::vec3(-1.0f, 0.0f, 0.0f));
	SetMaterial(MATERIAL::ANIMATED_MODEL);
	InitAnimation(MESH::ANIMATED_MODEL_IDLE);
	//SetScale(glm::vec3(0.2f));
	UpdateTransform();
	m_LastKnownPosition = position;

	//Test
	m_HasInjuryBoneBroken = 0.0f; // Random::GenerateBool();
	m_HasInjuryBurned = 0.0f; // Random::GenerateFloat(0.0f, 10.0f);
	m_HasInjurySmoke = 0.0f; // Random::GenerateFloat(0.0f, 10.0f);
	m_HasInjuryBleeding = 0.0f;
	m_Recovering = 0.0f;
	m_Resting = false;
	m_HasChangedTexture = false;
	/*m_SkillFire = Random::GenerateInt(1, 3);
	m_SkillMedic = Random::GenerateInt(1, 3);
	m_SkillStrength = Random::GenerateInt(1, 3);*/

	m_Health = MAX_HEALTH;
	m_MovementSpeed = CREWMEMBER_FULL_HEALTH_MOVEMENT_SPEED;
	m_Idling = true;

	m_Forgetfulness = 3;

	m_pAudioSourceScream = AudioSource::CreateSoundSource(SOUND::MONO_SCREAM);
	m_pAudioSourceScream->SetPosition(position);
	m_pAudioSourceScream->SetRollOffFactor(10.0f);
	m_pAudioSourceScream->SetReferenceDistance(1.0f);
	m_pAudioSourceScream->SetMaxDistance(100.0f);
	m_pAudioSourceScream->SetLooping(false);

	m_Group = groupType;

	m_CloseColor = DOOR_COLOR_RED;

	m_ReportTimer = 0.0f;
	m_ReportTime = 0.0f;

	SetPosition(position);
}

Crewmember::~Crewmember()
{
	DeleteSafe(m_pAudioSourceScream);
}

void Crewmember::SetRoom(uint32 room) noexcept
{
	if (m_pWorld->GetRoom(GetRoom()).IsActive())
	{
		if (GetRoom() != room && m_IsPicked)
		{
			m_pWorld->SetRoomActive(room, true, GetRoom(), true);
			m_pWorld->SetRoomActive(GetRoom(), false);
		}
	}

	GameObject::SetRoom(room);
}

void Crewmember::Update(const Camera& camera, float deltaTime) noexcept
{
	m_WasAbleToWork = IsAbleToWork();

	SceneGame* pSceneGame = Game::GetGame()->m_pSceneGame;
	m_OrderHandler.Update(pSceneGame, m_pWorld, pSceneGame->GetCrew(), deltaTime);
	GameObject::Update(camera, deltaTime);
	UpdateTransform();
	m_pShadow->UpdateTransform();

	if (IsAlive())
	{
		CheckSmokeDamage(m_pWorld->GetLevel(GetTile().y * 2 + 1).GetLevelData(), deltaTime);
		CheckFireDamage(m_pWorld->GetLevel(GetTile().y * 2).GetLevelData(), deltaTime);
		UpdateHealth(deltaTime);
	}

	Room& room = m_pWorld->GetRoom(GetRoom());

	uint32 index = m_pWorld->GetLevel(m_PlayerTile.y * 2).GetLevel()[m_PlayerTile.x][m_PlayerTile.z];

	if (room.IsBurning() && !room.IsFireDetected() && !room.IsFlooded())
	{
		room.SetFireDetected(true);
		Logger::LogEvent(GetName() + " larmar om eld i " + m_pWorld->GetNameFromGlobal(index) + "!", true);
		ReportPosition();
	}

	if (room.IsFlooded() && !room.IsFloodDetected())
	{
		room.SetFloodDetected(true);
		Logger::LogEvent(GetName() + " larmar om vatten i " + m_pWorld->GetNameFromGlobal(index) + "!", true); 
		ReportPosition();
	}

	if (m_pUISelectedCrew)
	{
		m_pUISelectedCrew->UpdatePosition(m_LastKnownPosition);
	}

	if (!IsAbleToWork() && !IsResting())
	{
		GoToSickBay();
	}
	
	ChangeTexture();

	if (!ReplayHandler::IsReplaying())
	{
		if (room.IsActive())
		{
			if (m_pShadow->IsVisible())
			{
				m_pShadow->SetIsVisible(false);
			}
			UpdateLastKnownPosition();
		}
		else
		{
			if (!m_pShadow->IsVisible())
			{
				m_pShadow->SetDirection(m_Direction);
				m_LastKnownPosition = m_PlayerTile * glm::ivec3(1, 2, 1);
				m_pShadow->SetPosition(m_LastKnownPosition);
				m_pShadow->UpdateTransform();
				m_pShadow->SetIsVisible(true);
			}
		}

		m_ReplayPositionSyncTimer += deltaTime;

		if (m_ReplayPositionSyncTimer > 0.25f)
		{
			new ReplayablePosition(GetPosition(), m_ShipNumber);
			m_ReplayPositionSyncTimer = 0.0f;
		}
	}
	else
	{
		UpdateLastKnownPosition();
	}

	m_pAudioSourceScream->SetPosition(GetPosition() + glm::vec3(pSceneGame->GetExtension() * glm::floor(GetPosition().y / 2), 0.0f, 0.0f));
}

void Crewmember::OnPicked(const std::vector<int32>& selectedMembers, int32 x, int32 y) noexcept
{
	if (!IsAbleToWalk() && !m_IsCarried)
	{
		AddChoice("Assistera", (void*)this);
		DisplayOrders(x, y, selectedMembers);
	}
	else
	{
		SetIsPicked(!m_IsPicked);
	}
}

void Crewmember::UpdateLastKnownPosition() noexcept
{
	m_LastKnownPosition = GetPosition();
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

		if (m_Idling)
		{
			movementSpeedMultiplier *= CREWMEMBER_IDLE_MOVEMENT_SPEED_MULTIPLIER;
		}
	}

	glm::vec3 res = GetPosition() + dir * m_MovementSpeed * movementSpeedMultiplier * dtS;
	SetPosition(res);

	if (m_pAssisting)
	{
		m_pAssisting->SetPosition(res);
	}
}

void Crewmember::FindPath(const glm::ivec3& goalPos)
{
	/*if (IsAbleToWalk())
	{
		m_OrderHandler.GiveOrder(new OrderWalk(goalPos), this);
	}
	else
	{
		Logger::LogEvent(GetName() + " cannot move!", true);
	}*/

	m_OrderHandler.GiveOrder(new OrderWalk(goalPos));
}

void Crewmember::SetCloseColor(uint32 doorColor)
{
	m_CloseColor = doorColor;
}

void Crewmember::GiveOrder(IOrder* order) noexcept
{
	if (!IsResting())
	{
		m_OrderHandler.GiveOrder(order);
	}
}

void Crewmember::OnOrderChosen(const std::string& name, void* userData, const std::vector<int32>& selectedMembers) noexcept
{
	if (name == "Assistera")
	{
		Crewmember* assister = nullptr;
		for (uint32 i = 0; i < selectedMembers.size(); i++)
		{
			assister = Game::GetGame()->m_pSceneGame->GetCrew()->GetMember(selectedMembers[i]);

			if (assister->IsIdling())
			{
				break;
			}
		}
		
		if (assister)
		{
			assister->GiveOrder(new OrderCarry(reinterpret_cast<Crewmember*>(userData)));
			m_IsCarried = true;
		}
	}
}

void Crewmember::LookForDoor() noexcept
{
	uint32 crewRoomIndex = m_pWorld->GetLevel(GetTile().y * 2).GetLevel()[GetTile().x][GetTile().z];
	for (int j = 0; j < m_pWorld->GetDoors().size(); j++)
	{
		glm::ivec3 doorTile = m_pWorld->GetDoor(j);
		uint32 doorRoomIndex = m_pWorld->GetLevel(doorTile.y).GetLevel()[doorTile.x][doorTile.z];
		if (doorRoomIndex == crewRoomIndex && doorTile != (m_PlayerTile * glm::ivec3(1, 2, 1)))
		{
			GameObjectDoor* door = (GameObjectDoor*)m_pWorld->GetLevel(doorTile.y).GetLevelData()[doorTile.x][doorTile.z].GameObjects[GAMEOBJECT_CONST_INDEX_DOOR];

			if (door->IsOpen() && door->GetColor() <= m_CloseColor)
			{
				m_OrderHandler.GiveOrder(new OrderDoor(door, doorTile, false));
			}
		}
	}
}

void Crewmember::GoToSickBay()
{
	if (!m_HasTriedToWalkToSickbay)
	{
		m_HasTriedToWalkToSickbay = true;

		if (IsAbleToWalk())
		{
			const glm::ivec3& currentTile = GetTile();
			uint32 currentTileID = m_pWorld->GetLevel(currentTile.y * 2).GetLevel()[currentTile.x][currentTile.z];

			if (currentTileID < SICKBAY_INTERVAL_START || currentTileID > SICKBAY_INTERVAL_END)
			{
				m_OrderHandler.GiveOrder(new OrderWalkMedicBay(currentTile));
			}
		}
		else
		{
			Logger::LogEvent(GetName() + " kan inte gå till sjukstugan!", true);
			ReportPosition();
		}
	}
}

bool Crewmember::Heal(float skillLevel, float dtS)
{
	bool res = false;
	/*//Broken Bone kan inte vara Bool
	if (HasInjuryBoneBroken())
	{
		m_HasInjuryBoneBroken -= (m_HasInjuryBoneBroken - m_HasInjuryBoneBroken / skillLevel) * dtS;
	}
	else if (HasInjuryBurned())
	{
		m_HasInjuryBurned -= (m_HasInjuryBurned - m_HasInjuryBurned / skillLevel) * dtS;
	}
	else if (HasInjurySmoke())
	{
		m_HasInjurySmoke -= (m_HasInjurySmoke - m_HasInjurySmoke / skillLevel) * dtS;
	}
	else if (HasInjuryBleed())
	{
		m_HasInjuryBleeding -= (m_HasInjuryBleeding - m_HasInjuryBleeding / skillLevel) * dtS;
	}
	else
	{
		res = true;
	}*/

	m_Recovering += skillLevel * dtS;
	if (HasRecovered())
	{
		Logger::LogEvent(GetName() + " har fått medicinsk hjälp!", true);
		ReportPosition();
		res = true;
	}
	return res;
}

void Crewmember::ApplyBurnInjury(float burn)
{
	bool lastState = HasInjuryBurned();
	m_HasInjuryBurned += burn;
	m_Recovering = 0.0f;
	if (lastState != HasInjuryBurned())
	{
		Logger::LogEvent(GetName() + " fick brännskador", false);
		m_pAudioSourceScream->Play();
	}
}

void Crewmember::ApplyBoneInjury(float boneBreak)
{
	bool lastState = HasInjuryBoneBroken();
	m_HasInjuryBoneBroken += boneBreak;
	m_Recovering = 0.0f;
	if (lastState != HasInjuryBoneBroken())
	{
		Logger::LogEvent(GetName() + " fick benbrott", false);
		m_pAudioSourceScream->Play();
	}
}

void Crewmember::ApplyBleedInjury(float bleed)
{
	bool lastState = HasInjuryBleed();
	m_HasInjuryBleeding += bleed;
	m_Recovering = 0.0f;
	if (lastState != HasInjuryBleed())
	{
		Logger::LogEvent(GetName() + " fick köttsår", false);
		m_pAudioSourceScream->Play();
	}
}

void Crewmember::ApplySmokeInjury(float smoke)
{
	bool lastState = HasInjurySmoke();
	m_HasInjurySmoke += smoke;
	m_Recovering = 0.0f;
	if (lastState != HasInjurySmoke())
	{
		Logger::LogEvent(GetName() + " fick rökskador", false);
		m_pAudioSourceScream->Play();
	}
}

int32 Crewmember::TestAgainstRay(const glm::vec3 ray, const glm::vec3 origin, float elevation, float extension) noexcept
{
	glm::vec3 centre = GetLastKnownPosition() + glm::vec3(0.0f, 0.9f, 0.0f);
	centre.x += extension * glm::clamp(glm::floor(centre.y / 2.0f), 0.0f, 2.0f);

	float t = -1;

	if (glm::floor(centre.y / 2) == glm::floor(elevation / 2) || extension > 0)
	{

		glm::vec3 normals[]{
			m_Direction,
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::normalize(glm::cross(normals[0], normals[1]))
		};

		float h[] = {
			0.15f,
			0.9f,
			0.25f
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
	}

	return (int32)t;
}

void Crewmember::OnOrderStarted(bool idleOrder) noexcept
{
	m_Idling = idleOrder;
}

void Crewmember::OnAllOrdersFinished() noexcept
{
	if (!ReplayHandler::IsReplaying())
	{
		SetIdling(true);

		UpdateAnimatedMesh(MESH::ANIMATED_MODEL_IDLE);
		m_HasTriedToWalkToSickbay = false;

		GiveOrder(OrderSchedule::GetIdleOrder());
	}
}

void Crewmember::OnAddedToScene(Scene* pScene) noexcept
{
	//Look at
	{
		m_pShadow = new GameObject();
		m_pShadow->SetMaterial(MATERIAL::BLACK);
		m_pShadow->SetMesh(MESH::CREWMEMBER_SHADOW);
		m_pShadow->SetPosition(m_LastKnownPosition);
		m_pShadow->SetDirection(m_Direction);
		m_pShadow->UpdateTransform();
		m_pShadow->SetWorld(m_pWorld);
		m_pShadow->SetIsVisible(!ReplayHandler::IsReplaying());
		pScene->AddGameObject(m_pShadow);
	}

	pScene->RegisterPickableGameObject(this);
}

void Crewmember::OnHovered() noexcept
{
	m_IsHovered = true;
	Game::GetGame()->m_pSceneGame->GetUICrewMember()->SetCrewMember(this);
}

void Crewmember::OnNotHovered() noexcept
{
	m_IsHovered = false;
	Game::GetGame()->m_pSceneGame->GetUICrewMember()->SetCrewMember(nullptr);
}

void Crewmember::SetPosition(const glm::vec3& position) noexcept
{ 
	glm::vec3 pos = glm::vec3(glm::clamp(position.x, 1.0f, 10.0f), glm::clamp(position.y, 0.0f, 4.0f), glm::clamp(position.z, 1.0f, 40.0f));
	m_PlayerTile = glm::ivec3(std::round(position.x), std::floor((position.y) / 2), std::round(position.z));
	//m_PlayerTile.z = m_PlayerTile.z < 0 ? 0 : m_PlayerTile.z;
	
	//HOT FIX (Gay?)
	m_PlayerTile.y = (m_PlayerTile.y >= 3) ? 2 : m_PlayerTile.y;

	if (m_PlayerTile.x >= 0 && m_PlayerTile.x <= 11)
	{
		SetRoom(m_pWorld->GetLevel(m_PlayerTile.y * 2).GetLevel()[m_PlayerTile.x][m_PlayerTile.z]);
	}

	GameObject::SetPosition(position);
	if (m_pAudioSourceScream)
	{
		m_pAudioSourceScream->SetPosition(position);
	}
}

void Crewmember::SetIsPicked(bool picked) noexcept
{
	if (m_IsPicked != picked)
	{
		m_IsPicked = picked;

		if (m_IsPicked)
		{
			m_pUISelectedCrew = new UISelectedCrew(GetName());
			m_pUISelectedCrew->AddProgressButtonListener(this);
			Game::GetGame()->GetGUIManager().Add(m_pUISelectedCrew);
			Game::GetGame()->m_pSceneGame->GetCrew()->AddToSelectedList(GetShipNumber());
		}
		else
		{
			Game::GetGame()->GetGUIManager().Remove(m_pUISelectedCrew);
			Game::GetGame()->m_pSceneGame->GetCrew()->RemoveFromSelectedList(GetShipNumber());
			m_pUISelectedCrew = nullptr;
		}
	}
}

void Crewmember::SetDirection(const glm::vec3& direction) noexcept
{
	m_Direction = glm::normalize(direction);
	float angle = std::atan2f(1.0f * m_Direction.x, -1.0f * m_Direction.z);
	SetRotation(glm::vec4(0.0f, 1.0f, 0.0f, -angle));
}

void Crewmember::SetAssisting(Crewmember* inNeed) noexcept
{
	m_pAssisting = inNeed;
}

void Crewmember::SetIdling(bool value) noexcept
{
	m_Idling = value;
	if (!IsAbleToWork())
	{
		m_Idling = false;
	}
}

void Crewmember::SetGearIsEquipped(bool value) noexcept
{
	m_GearIsEquipped = value;
}

void Crewmember::SetExtinguisherIsEquipped(bool value) noexcept
{
	m_HasEquippedExtinguisher = value;
}

void Crewmember::SetResting(bool value) noexcept
{
	m_Resting = value;
}

void Crewmember::ReportPosition() noexcept
{
	m_LastKnownPosition = GetPosition();
	m_pWorld->SetRoomActive(GetRoom(), true);
}

void Crewmember::RequestReportPosition() noexcept
{
	if (m_pUISelectedCrew && m_pUISelectedCrew->GetPercentage() >= 1.0f)
	{
		m_pUISelectedCrew->SetPercentage(0.0f);
		m_pUISelectedCrew->StartAnimation((float)Random::GenerateInt(MIN_REPORT_TIME, MAX_REPORT_TIME));
	}
}

void Crewmember::ChangeTexture() noexcept
{
	if (!IsAbleToWalk() && !m_HasChangedTexture)
	{
		SetMaterial(MATERIAL::CREW_INJURED);
		m_HasChangedTexture = true;
	}
}

void Crewmember::UpdateHealth(float dt)
{
	//Tweak here!
	float smokeDmgSpeed = 1.0f;
	float burnDmgSpeed = 1.0f;
	float bleedDmgSpeed = 1.0f;
	if (!HasRecovered())
	{
		if (HasInjurySmoke())
		{
			m_Health -= (float)(std::log10(m_HasInjurySmoke) - std::log10(1.0)) * smokeDmgSpeed * dt;
		}

		if (HasInjuryBurned())
		{
			m_Health -= (float)(std::log10(m_HasInjuryBurned) - std::log10(1.0)) * burnDmgSpeed * dt;
		}

		if (HasInjuryBleed())
		{
			m_Health -= (float)(std::log10(m_HasInjuryBleeding) - std::log10(1.0)) * bleedDmgSpeed * dt;
		}
	}

	if (m_Health <= 0.0f)
	{
		Logger::LogEvent(GetName() + " har svimmat!", false);
		m_MovementSpeed = CREWMEMBER_DEAD_MOVEMENT_SPEED;
		UpdateAnimatedMesh(MESH::ANIMATED_MODEL_SLEEP);
		
	}
	else if (m_Health < MAX_HEALTH)
	{
		if (m_Health > 0.5f * MAX_HEALTH)
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

	if (!IsAbleToWork())
	{
		if (IsIdling())
		{
			GoToSickBay();
		}

		if (m_WasAbleToWork)
		{
			GameState::SetCrewHealth(GameState::GetCrewHealth() - (1.0f / NUM_CREW));
			std::cout << "Crewmemeber marked as incapacitated" << std::endl;
		}
	}
}

void Crewmember::CheckSmokeDamage(const TileData* const * data, float dt) noexcept
{
	if (m_Group == SMOKE_DIVER && m_GearIsEquipped)
	{
		return;
	}

	//HOT FIX
	if (m_PlayerTile.z < 0)
	{
		return;
	}

	float smokeDmgSpeed = 0.1f;
	const TileData& tileData = data[m_PlayerTile.x][m_PlayerTile.z];
	if (tileData.SmokeAmount - tileData.SmokeLimit >= 1.0)
	{
		ApplySmokeInjury((tileData.SmokeAmount / tileData.SmokeLimit) * smokeDmgSpeed * dt);
	}
}

void Crewmember::CheckFireDamage(const TileData * const * data, float dt) noexcept
{
	if (m_Group == SMOKE_DIVER && m_GearIsEquipped)
	{
		return;
	}

	//HOT FIX
	if (m_PlayerTile.z < 0)
	{
		return;
	}

	float burnSpeed = 0.1f;
	const TileData& tileData = data[m_PlayerTile.x][m_PlayerTile.z];
	if (tileData.Temp >= tileData.BurnsAt)
	{
		ApplyBurnInjury((tileData.Temp / tileData.BurnsAt) * burnSpeed * dt);
	}
}

void Crewmember::SetShipNumber(int32 shipnumber) noexcept
{
	m_ShipNumber = shipnumber;
}

void Crewmember::CloseDoorOrder(glm::ivec3 doorTile)
{
	GameObjectDoor* door = (GameObjectDoor*)m_pWorld->GetLevel(doorTile.y).GetLevelData()[doorTile.x][doorTile.z].GameObjects[GAMEOBJECT_CONST_INDEX_DOOR];
	m_OrderHandler.GiveOrder(new OrderDoor(door, doorTile, false));
}
