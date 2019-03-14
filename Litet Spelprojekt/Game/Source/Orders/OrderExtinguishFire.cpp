#include "..\..\Include\Orders\OrderExtinguishFire.h"
#include "../../Include/Crewmember.h"

OrderExtinguishFire::OrderExtinguishFire(const glm::ivec3& roomTile, const glm::ivec3& burningTile, uint32 roomBurningId, bool hasGearEquipped, bool fireFullyExtinguished) :
	OrderWalk(hasGearEquipped != fireFullyExtinguished ? burningTile : roomTile),
	m_EquippingGearTimer(0.0f),
	m_ExtinguishingIntensity(FIRE_EXTINGUISH_BY_CREW_RATE)
{
	//hasGearEquipped T fireFullyExtinguished F : T
	//hasGearEquipped F fireFullyExtinguished T : ~
	//hasGearEquipped F fireFullyExtinguished F : F
	//hasGearEquipped T fireFullyExtinguished T : F

	m_RoomBurningId = roomBurningId;
	m_RoomTile = roomTile;
	m_BurningTile = burningTile;
	m_ExtinguishingFire = false;
	m_FireFullyExtinguished = fireFullyExtinguished;
	std::cout << "Extinguish Fire Order Room Tile: " << glm::to_string(roomTile) << std::endl;
	std::cout << "Extinguish Fire Order Burning Tile: " << glm::to_string(burningTile) << std::endl;
}

OrderExtinguishFire::~OrderExtinguishFire()
{
}

void OrderExtinguishFire::OnStarted(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept
{
	OrderWalk::OnStarted(pScene, pWorld, pCrewMembers);
}

bool OrderExtinguishFire::OnUpdate(Scene* pScene, World* pWorld, Crew* pCrewMembers, float dtS) noexcept
{
	Crewmember* pCrewmember = GetCrewMember();

	if (!m_FireFullyExtinguished)
	{
		if (!pCrewmember->HasGearEquipped())
		{
			//Run To Room
			if (OrderWalk::OnUpdate(pScene, pWorld, pCrewMembers, dtS))
			{
				if (m_EquippingGearTimer <= 0.00001f)
				{
					pCrewmember->UpdateAnimatedMesh(MESH::ANIMATED_MODEL_OPENDOOR);
				}

				//Equip Gear
				if ((m_EquippingGearTimer += dtS) > TIME_TO_EQUIP_GEAR)
				{
					m_EquippingGearTimer = 0.0f;
					pCrewmember->SetGearIsEquipped(true);
					Logger::LogEvent(GetCrewMember()->GetName() + " satte på sig rökdykarutrustning!", true);
					pCrewmember->GiveOrder(new OrderExtinguishFire(m_RoomTile, m_BurningTile, m_RoomBurningId, pCrewmember->HasGearEquipped(), false));
					return false;
				}
			}
		}
		else
		{
			//Run To Room That is Burning
			if (OrderWalk::OnUpdate(pScene, pWorld, pCrewMembers, dtS))
			{
				const glm::ivec2& levelSize = glm::ivec2(pWorld->GetLevel(m_BurningTile.y).GetSizeX(), pWorld->GetLevel(m_BurningTile.y).GetSizeZ());
				const uint32 * const * ppLevel = pWorld->GetLevel(m_BurningTile.y).GetLevel();
				TileData * const * ppLevelData = pWorld->GetLevel(m_BurningTile.y).GetLevelData();
				TileData& burningTile = ppLevelData[m_BurningTile.x][m_BurningTile.z];

				if (!m_ExtinguishingFire)
				{
					m_ExtinguishingFire = true;
				}

				glm::vec3 direction = GetCrewMember()->GetDirection();
				glm::vec3 right = glm::normalize(glm::cross(UP_VECTOR, GetCrewMember()->GetDirection()));

				glm::ivec3 tileR = m_BurningTile + glm::ivec3(glm::round(direction + right));
				glm::ivec3 tileM = m_BurningTile + glm::ivec3(glm::round(direction));
				glm::ivec3 tileL = m_BurningTile + glm::ivec3(glm::round(direction - right));

				bool tileRInWorld = CheckIfTileInWorld(levelSize, tileR);
				bool tileMInWorld = CheckIfTileInWorld(levelSize, tileM);
				bool tileLInWorld = CheckIfTileInWorld(levelSize, tileL);

				bool tileTargetExtinguished = ExtinguishIfInWorld(ppLevelData, m_BurningTile, true, dtS);
				bool tileRExtinguished = ExtinguishIfInWorld(ppLevelData, tileR, tileRInWorld, dtS);
				bool tileMExtinguished = ExtinguishIfInWorld(ppLevelData, tileM, tileMInWorld, dtS);
				bool tileLExtinguished = ExtinguishIfInWorld(ppLevelData, tileL, tileLInWorld, dtS);

				bool allFiresExtinguished =
					tileTargetExtinguished &&
					tileRExtinguished &&
					tileMExtinguished &&
					tileLExtinguished;

				if (allFiresExtinguished)
				{
					m_ExtinguishingFire = false;
					pCrewmember->UpdateAnimatedMesh(MESH::ANIMATED_MODEL_RUN);
					glm::ivec2 newTarget = FindClosestBurningTile(ppLevel, ppLevelData, levelSize, glm::ivec2(m_BurningTile.x, m_BurningTile.z));

					std::cout << "New Target: " << glm::to_string(newTarget) << std::endl;

					//Fire Fully Extinguished
					if (newTarget.x == -1 && newTarget.y == -1)
					{
						m_FireFullyExtinguished = true;
						m_BurningTile = glm::ivec3(0);
						m_RoomBurningId = 0;
						Logger::LogEvent(GetCrewMember()->GetName() + " blev färdig med eldsläckning!", true);
						pCrewmember->GiveOrder(new OrderExtinguishFire(m_RoomTile, m_BurningTile, m_RoomBurningId, pCrewmember->HasGearEquipped(), true));
						return false;
					}

					//Fire Not Fully Extinguished
					m_BurningTile = glm::ivec3(newTarget.x, m_BurningTile.y, newTarget.y);
					m_RoomBurningId = ppLevel[newTarget.x][newTarget.y];
					pCrewmember->GiveOrder(new OrderExtinguishFire(m_RoomTile, m_BurningTile, m_RoomBurningId, pCrewmember->HasGearEquipped(), false));
					return false;
				}
				else
				{
					m_ExtinguishingIntensity * 1.1f;
				}
			}
			else
			{
				//Check if we encounter fire on the way
				glm::ivec3 correctTargetTile = glm::ivec3(m_TargetTile.x, 2 * m_TargetTile.y, m_TargetTile.z);

				const uint32 * const * ppLevel = pWorld->GetLevel(correctTargetTile.y).GetLevel();
				TileData& targetTile = pWorld->GetLevel(correctTargetTile.y).GetLevelData()[m_TargetTile.x][m_TargetTile.z];

				if (targetTile.Temp > targetTile.BurnsAt && m_BurningTile != correctTargetTile)
				{
					m_BurningTile = correctTargetTile;
					m_RoomBurningId = ppLevel[correctTargetTile.x][correctTargetTile.z];
					pCrewmember->GiveOrder(new OrderExtinguishFire(m_RoomTile, m_BurningTile, m_RoomBurningId, pCrewmember->HasGearEquipped(), false));
					return false;
				}
			}
		}
	}
	else
	{
		if (pCrewmember->HasGearEquipped())
		{
			//Run To Room
			if (OrderWalk::OnUpdate(pScene, pWorld, pCrewMembers, dtS))
			{
				if (m_EquippingGearTimer <= 0.00001f)
				{
					pCrewmember->UpdateAnimatedMesh(MESH::ANIMATED_MODEL_OPENDOOR);
				}

				//Equip Gear
				if ((m_EquippingGearTimer += dtS) > TIME_TO_EQUIP_GEAR)
				{
					if (ReadyToAbort())
					{
						m_EquippingGearTimer = 0.0f;
						pCrewmember->SetGearIsEquipped(false);
						Logger::LogEvent(GetCrewMember()->GetName() + " tog av sig rökdykarutrustning!", true);
						return true;
					}
				}
			}
		}
	}

	//Run out if dying or our fire out of control

	return false;
}

void OrderExtinguishFire::OnEnded(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept
{
	OrderWalk::OnEnded(pScene, pWorld, pCrewMembers);

	GetCrewMember()->ReportPosition();
}

bool OrderExtinguishFire::CanBeStackedWithSameType() noexcept
{
	return false;
}

std::string OrderExtinguishFire::GetName() noexcept
{
	return "Extinguish Fire";
}

bool OrderExtinguishFire::IsIdleOrder() noexcept
{
	return false;
}

bool OrderExtinguishFire::CanExecuteIfHurt() noexcept
{
	return false;
}

bool OrderExtinguishFire::HasPriority() noexcept
{
	return true;
}
