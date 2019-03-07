#pragma once
#include "OrderWalk.h"
#include <World/World.h>

#define TIME_TO_EQUIP_GEAR 5.0f
#define FIRE_EXTINGUISH_TEMP_REDUCTION 100.0f
#define FIRE_EXTINGUISH_TEMP_REDUCTION_PERCENTAGE 0.5f

class OrderExtinguishFire : public OrderWalk
{
public:
	OrderExtinguishFire(const glm::ivec3& roomTile, const glm::ivec3& burningTile, uint32 roomBurningId, bool hasGearEquipped);
	virtual ~OrderExtinguishFire();

	virtual void StartOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept override;
	virtual bool UpdateOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers, float dtS) noexcept override;
	virtual void EndOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept override;
	virtual void AbortOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept override;
	virtual bool AllowsMultipleOrders() noexcept override;
	virtual std::string GetName() noexcept override;
	virtual bool IsIdleOrder() noexcept override;
	virtual bool CanExecuteIfHurt() noexcept override;

protected:
	bool CheckIfTileInWorld(const glm::ivec2& levelSize, const glm::ivec3& tile) const noexcept;

private:
	bool ExtinguishIfInWorld(TileData * const * ppLevelData, const glm::ivec3& tile, bool inWorld) const noexcept;
	glm::ivec2 FindClosestBurningTile(const uint32 * const * ppLevel, TileData * const * ppLevelData, const glm::ivec2& levelSize, const glm::ivec2& currentTile) const noexcept;

private:
	uint32 m_RoomBurningId;
	glm::ivec3 m_RoomTile;
	glm::ivec3 m_BurningTile;
	float m_EquippingGearTimer;
	bool m_ExtinguishingFire;
	bool m_FireFullyExtinguished;
};

inline bool OrderExtinguishFire::CheckIfTileInWorld(const glm::ivec2& levelSize, const glm::ivec3& tile) const noexcept
{
	if (tile.x < 1 || tile.x > levelSize.x - 1 || tile.z < 1 || tile.z > levelSize.y - 1)
	{
		return false;
	}

	return true;
}

inline bool OrderExtinguishFire::ExtinguishIfInWorld(TileData * const * ppLevelData, const glm::ivec3& tile, bool inWorld) const noexcept
{
	if (!inWorld)
	{
		return true;
	}

	if (ppLevelData[tile.x][tile.z].Temp > ppLevelData[tile.x][tile.z].BurnsAt)
	{
		ppLevelData[tile.x][tile.z].Temp = -273.15f;
	}

	return ppLevelData[tile.x][tile.z].Temp < ppLevelData[tile.x][tile.z].BurnsAt;
}

inline glm::ivec2 OrderExtinguishFire::FindClosestBurningTile(const uint32 * const * ppLevel, TileData * const * ppLevelData, const glm::ivec2& levelSize, const glm::ivec2& currentTile) const noexcept
{
	
	glm::ivec2 closestBurningTileInRoom(-1);
	glm::ivec2 closestBurningTileNotInRoom(-1);

	uint32 closestDistanceToBurningTileInRoomSqrd = UINT32_MAX;
	uint32 closestDistanceToBurningTileNotInRoomSqrd = UINT32_MAX;

	glm::ivec2 currentTileBeingChecked(1);
	for (; currentTileBeingChecked.x < levelSize.x - 1; currentTileBeingChecked.x++)
	{
		for (; currentTileBeingChecked.y < levelSize.y - 1; currentTileBeingChecked.y++)
		{
			glm::ivec2 toVector = currentTile - currentTileBeingChecked;
			uint32 distSqrdToCheck = (uint32)(toVector.x * toVector.x + toVector.y * toVector.y);

			if (ppLevelData[currentTileBeingChecked.x][currentTileBeingChecked.y].Temp > ppLevelData[currentTileBeingChecked.x][currentTileBeingChecked.y].BurnsAt)
			{
				if (m_RoomBurningId == ppLevel[currentTileBeingChecked.x][currentTileBeingChecked.y])
				{
					if (distSqrdToCheck < closestDistanceToBurningTileInRoomSqrd)
					{
						closestDistanceToBurningTileInRoomSqrd = distSqrdToCheck;
						closestBurningTileInRoom = currentTileBeingChecked;
					}
				}
				else
				{
					if (distSqrdToCheck < closestDistanceToBurningTileNotInRoomSqrd)
					{
						closestDistanceToBurningTileNotInRoomSqrd = distSqrdToCheck;
						closestBurningTileNotInRoom = currentTileBeingChecked;
					}
				}
			}
		}

		currentTileBeingChecked.y = 1;
	}

	if (closestBurningTileInRoom.x != -1 && closestBurningTileInRoom.y != -1)
	{
		return closestBurningTileInRoom;
	}

	return closestBurningTileNotInRoom;
}
