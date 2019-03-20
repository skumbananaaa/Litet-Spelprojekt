#pragma once
#include "OrderWalk.h"
#include <World/World.h>
#include "OrderSchedule.h"
#include "../Game.h"

#define TIME_TO_EQUIP_GEAR 5.0f
#define FIRE_EXTINGUISH_TEMP_REDUCTION 100.0f
#define FIRE_EXTINGUISH_TEMP_REDUCTION_PERCENTAGE 0.5f

class OrderExtinguishFire : public OrderWalk
{
	static constexpr float FIRE_EXTINGUISH_BY_CREW_RATE = 40.0f * RATE_OF_FIRE_SPREAD;

public:
	OrderExtinguishFire(OrderExtinguishFire* other);
	OrderExtinguishFire(const glm::ivec3& goalTile, const glm::ivec3& burningTile, uint32 roomBurningId, bool fireFullyExtinguished, const std::string& extinguisherName);
	virtual ~OrderExtinguishFire();

	virtual void OnStarted(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept override;
	virtual void OnEnded(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept override;
	virtual bool OnUpdate(Scene* pScene, World* pWorld, Crew* pCrewMembers, float dtS) noexcept override;
	virtual bool CanBeStackedWithSameType() noexcept override;
	virtual std::string GetName() noexcept override;
	virtual bool IsIdleOrder() noexcept override;
	virtual bool CanExecuteIfHurt() noexcept override;
	virtual bool HasPriority() noexcept override;

	virtual IOrder* Clone() noexcept override;
	virtual void InitClone(SceneGame* pScene, void* userData) noexcept override;

protected:
	bool CheckIfTileInWorld(const glm::ivec2& levelSize, const glm::ivec3& tile) const noexcept;

private:
	bool ExtinguishIfInWorld(TileData * const * ppLevelData, const glm::ivec3& tile, bool inWorld, float dtS) const noexcept;
	glm::ivec2 FindClosestBurningTile(World* pWorld, uint32 level, const glm::ivec2& levelSize, const glm::ivec2& currentTile) const noexcept;
	glm::ivec3 FindClosestExtinguisher(const glm::vec3& currentPosition, std::string& extinguisherName) noexcept;

private:
	uint32 m_RoomBurningId;
	glm::ivec3 m_BurningTile;
	float m_EquippingGearTimer;
	float m_ExtinguishingIntensity;
	bool m_ExtinguishingFire;
	bool m_FireFullyExtinguished;
	std::string m_ExtinguisherName;
};

inline bool OrderExtinguishFire::CheckIfTileInWorld(const glm::ivec2& levelSize, const glm::ivec3& tile) const noexcept
{
	if (tile.x < 1 || tile.x > levelSize.x - 1 || tile.z < 1 || tile.z > levelSize.y - 1)
	{
		return false;
	}

	return true;
}

inline bool OrderExtinguishFire::ExtinguishIfInWorld(TileData * const * ppLevelData, const glm::ivec3& tile, bool inWorld, float dtS) const noexcept
{
	if (!inWorld)
	{
		return true;
	}

	TileData& tileData = ppLevelData[tile.x][tile.z];

	if (tileData.Temp > tileData.BurnsAt)
	{
		tileData.Temp -= m_ExtinguishingIntensity * dtS;
		tileData.BurnsAt *= 1.1f;
		tileData.BurnsAt = glm::min(tileData.BurnsAt, MAX_TILE_TEMP - 10.0f);

		if (tileData.Temp < tileData.BurnsAt)
		{
			if (tileData.GameObjects[GAMEOBJECT_CONST_INDEX_FLOOR] != nullptr)
			{
				tileData.GameObjects[GAMEOBJECT_CONST_INDEX_FLOOR]->SetMaterial(
					World::ConvertNonExtToExtFloorMaterial(tileData.GameObjects[GAMEOBJECT_CONST_INDEX_FLOOR]->GetMaterial()));
			}
		}
	}

	return ppLevelData[tile.x][tile.z].Temp < ppLevelData[tile.x][tile.z].BurnsAt;
}

inline glm::ivec2 OrderExtinguishFire::FindClosestBurningTile(World* pWorld, uint32 level, const glm::ivec2& levelSize, const glm::ivec2& currentTile) const noexcept
{

	const uint32 * const * ppLevel = pWorld->GetLevel(level).GetLevel();
	TileData * const * ppLevelData = pWorld->GetLevel(level).GetLevelData();
	
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

			TileData& currentTileData = ppLevelData[currentTileBeingChecked.x][currentTileBeingChecked.y];

			if (currentTileData.Temp > currentTileData.BurnsAt && currentTileData.GameObjects[GAMEOBJECT_CONST_INDEX_FIRE]->IsVisible() && !currentTileData.MarkedForExtinguish)
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


inline glm::ivec3 OrderExtinguishFire::FindClosestExtinguisher(const glm::vec3& currentPosition, std::string& extinguisherName) noexcept
{
	float shortDistance = FLT_MAX;
	int index = -1;

	for (int i = 0; i < OrderSchedule::s_Extinguishers.size(); i++)
	{
		glm::vec3 temp = OrderSchedule::s_Extinguishers[i]->GetPosition() - currentPosition;
		temp.y *= 10.0f;
		float currentDistanceSqrd = glm::length2(temp);
		
		if (currentDistanceSqrd < shortDistance)
		{
			index = i;
			shortDistance = currentDistanceSqrd;
		}
	}

	GameObject* exting = nullptr;
	exting = OrderSchedule::s_Extinguishers[index];
	OrderSchedule::s_Extinguishers.erase(OrderSchedule::s_Extinguishers.begin() + index);
	extinguisherName = exting->GetName();
	if (extinguisherName == "")
	{
		std::cout << "ERROR: invalid extinguisher at index '" << index << '\'' << std::endl;
	}

	return exting->GetTile();
}