#include "..\..\Include\Orders\OrderExtinguishFire.h"
#include "../../Include/Crewmember.h"

OrderExtinguishFire::OrderExtinguishFire(const glm::ivec3& roomTile, const glm::ivec3& burningTile, uint32 roomBurningId, bool hasGearEquipped) :
	OrderWalk(hasGearEquipped ? burningTile : roomTile),
	m_EquippingGearTimer(0.0f)
{
	m_RoomBurningId = roomBurningId;
	m_RoomTile = roomTile;
	m_BurningTile = burningTile;
	m_ExtinguishingFire = false;
	std::cout << "Extinguish Fire Order Room Tile: " << glm::to_string(roomTile) << std::endl;
	std::cout << "Extinguish Fire Order Burning Tile: " << glm::to_string(burningTile) << std::endl;
}

OrderExtinguishFire::~OrderExtinguishFire()
{
}

void OrderExtinguishFire::StartOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept
{
	OrderWalk::StartOrder(pScene, pWorld, pCrewMembers);
}

bool OrderExtinguishFire::UpdateOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers, float dtS) noexcept
{
	Crewmember* pCrewMember = GetCrewMember();

	if (!pCrewMember->HasGearEquipped())
	{
		//Run To Room
		if (OrderWalk::UpdateOrder(pScene, pWorld, pCrewMembers, dtS))
		{
			if (m_EquippingGearTimer <= 0.00001f)
			{
				pCrewMember->UpdateAnimatedMesh(MESH::ANIMATED_MODEL_OPENDOOR);
			}

			//Equip Gear
			if ((m_EquippingGearTimer += dtS) > TIME_TO_EQUIP_GEAR)
			{
				m_EquippingGearTimer = 0.0f;
				pCrewMember->SetGearIsEquipped(true);
				//pCrewMember->GiveOrder(new OrderExtinguishFire(m_RoomTile, m_BurningTile, m_RoomBurningId, true));
				OrderWalk::RestartOrder(pScene, pWorld, pCrewMembers, m_BurningTile);
				return false;
			}
		}
	}
	else
	{
		//Run To Room That is Burning
		if (OrderWalk::UpdateOrder(pScene, pWorld, pCrewMembers, dtS))
		{
			const glm::ivec2& levelSize = glm::ivec2(pWorld->GetLevel(m_BurningTile.y).GetSizeX(), pWorld->GetLevel(m_BurningTile.y).GetSizeZ());
			const uint32 * const * ppLevel = pWorld->GetLevel(m_BurningTile.y).GetLevel();
			TileData * const * ppLevelData = pWorld->GetLevel(m_BurningTile.y).GetLevelData();
			TileData& burningTile = ppLevelData[m_BurningTile.x][m_BurningTile.z];

			if (!m_ExtinguishingFire)
			{
				m_ExtinguishingFire = true;
				pCrewMember->UpdateAnimatedMesh(MESH::ANIMATED_MODEL_SLEEP);
			}

			glm::ivec3 tilePosX = glm::ivec3(m_BurningTile.x + 1, m_BurningTile.y, m_BurningTile.z);
			glm::ivec3 tileNegX = glm::ivec3(m_BurningTile.x - 1, m_BurningTile.y, m_BurningTile.z);
			glm::ivec3 tilePosZ = glm::ivec3(m_BurningTile.x, m_BurningTile.y, m_BurningTile.z + 1);
			glm::ivec3 tileNegZ = glm::ivec3(m_BurningTile.x, m_BurningTile.y, m_BurningTile.z - 1);

			bool tilePosXInWorld = CheckIfTileInWorld(levelSize, tilePosX);
			bool tileNegXInWorld = CheckIfTileInWorld(levelSize, tileNegX);
			bool tilePosZInWorld = CheckIfTileInWorld(levelSize, tilePosZ);
			bool tileNegZInWorld = CheckIfTileInWorld(levelSize, tileNegZ);

			bool tileTargetExtinguished = ExtinguishIfInWorld(ppLevelData, m_BurningTile, true);
			bool tilePosXExtinguished = ExtinguishIfInWorld(ppLevelData, tilePosX, tilePosXInWorld);
			bool tileNegXExtinguished = ExtinguishIfInWorld(ppLevelData, tileNegX, tileNegXInWorld);
			bool tilePosZExtinguished = ExtinguishIfInWorld(ppLevelData, tilePosZ, tilePosZInWorld);
			bool tileNegZExtinguished = ExtinguishIfInWorld(ppLevelData, tileNegZ, tileNegZInWorld);

			bool allFiresExtinguished =
				tileTargetExtinguished &&
				tilePosXExtinguished &&
				tileNegXExtinguished &&
				tilePosZExtinguished &&
				tileNegZExtinguished;

			if (allFiresExtinguished)
			{
				m_ExtinguishingFire = false;
				pCrewMember->UpdateAnimatedMesh(MESH::ANIMATED_MODEL_RUN);
				glm::ivec2 newTarget = FindClosestBurningTile(ppLevel, ppLevelData, levelSize, glm::ivec2(m_BurningTile.x, m_BurningTile.z));

				std::cout << "New Target: " << glm::to_string(newTarget) << std::endl;

				if (newTarget.x == -1 && newTarget.y == -1)
				{
					return true;
				}

				m_BurningTile = glm::ivec3(newTarget.x, m_BurningTile.y, newTarget.y);
				m_RoomBurningId = ppLevel[newTarget.x][newTarget.y];
				OrderWalk::RestartOrder(pScene, pWorld, pCrewMembers, m_BurningTile);
			}

			//Extinguish Fire
		}
		else
		{
			glm::ivec3 correctTargetTile = glm::ivec3(m_TargetTile.x, 2 * m_TargetTile.y, m_TargetTile.z);

			const uint32 * const * ppLevel = pWorld->GetLevel(correctTargetTile.y).GetLevel();
			TileData& targetTile = pWorld->GetLevel(correctTargetTile.y).GetLevelData()[m_TargetTile.x][m_TargetTile.z];

			if (targetTile.Temp > targetTile.BurnsAt && m_BurningTile != correctTargetTile)
			{
				m_BurningTile = correctTargetTile;
				m_RoomBurningId = ppLevel[correctTargetTile.x][correctTargetTile.z];
				OrderWalk::RestartOrder(pScene, pWorld, pCrewMembers, m_BurningTile);
				return false;
			}
		}
	}

	return false;

	//Run out if dying or our fire out of control
}

void OrderExtinguishFire::EndOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept
{
	OrderWalk::EndOrder(pScene, pWorld, pCrewMembers);
}

void OrderExtinguishFire::AbortOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept
{
	OrderWalk::AbortOrder(pScene, pWorld, pCrewMembers);
}

bool OrderExtinguishFire::AllowsMultipleOrders() noexcept
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
