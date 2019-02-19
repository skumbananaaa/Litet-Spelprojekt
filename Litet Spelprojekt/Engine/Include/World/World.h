#pragma once
#include <EnginePch.h>
#include <World/WorldLevel.h>
#include <World/Room.h>
#include <Graphics/Scene.h>
#include <System/Input.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <GLM/gtx/string_cast.hpp>

struct WorldObject
{
	glm::uvec3 TileId = glm::uvec3(0);
	int32 GameObject = -1;
	float Rotation;
};

class API World
{
public:
	World(World&& other) = delete;
	World(const World& other) = delete;
	World& operator=(World&& other) = delete;
	World& operator=(const World& other) = delete;

	///<summary>Summary: Each level is a ship level, not a grid level. This object deletes the worldlevel and worldobject objects on deletion.</summary>
	World(WorldLevel* worldLevels[], uint32 numLevels, WorldObject* objects = nullptr, uint32 numObjects = 0) noexcept;
	~World();

	void AddWorldObject(const WorldObject& object) noexcept;
	void SetStairs(const glm::ivec3* stairs, uint32 nrOfStairs);

	const WorldLevel* const GetLevel(uint32 level) const noexcept;
	const WorldObject& GetWorldObject(uint32 index) const noexcept;
	uint32 GetNumLevels() const noexcept;
	uint32 GetNumWorldObjects() const noexcept;

	const glm::ivec3* GetStairs() const noexcept;
	uint32 GetNumStairs() const noexcept;
	Room* GetRoom(uint32 room) const noexcept;

	void GenerateRooms();
	void GenerateWater(Scene* pScene) noexcept;

	void Update(Scene* pScene, float dt);

private:
	//Scenario Spread Helper Functions
	uint32 CanSpreadTo(const uint32 * const * ppLevel, const glm::ivec2& levelSize, const glm::ivec2& tileFrom, const glm::ivec2& tileTo) const noexcept;
	
	//Water Scenario Helper Functions
	float CanFloodTo(const uint32 * const * ppLevel, const TileData * const * ppLevelData, const glm::ivec2& levelSize, const glm::ivec2& tileFrom, const glm::ivec2& tileTo, uint32 canSpreadTo) const noexcept;
	float CalculateFloodFactor(float waterLevelDifPosX, float waterLevelDifNegX, float waterLevelDifPosZ, float waterLevelDifNegZ, float dtS) const noexcept;

	void UpdateFloodingIds(TileData * const * ppLevelData, std::vector<glm::ivec2>& newFloodingIDs, const glm::ivec2& tilePos, uint32 canSpreadToo) const noexcept;
	void UpdateWaterLevel(TileData * const * ppLevelData, const glm::ivec2& tileFrom, const glm::ivec2& tileTo, float floodFactor, float waterLevelDif) const noexcept;

	void UpdateFloodingIdsBelow(WorldLevel* pWorldLevel, const glm::ivec2& tile) const noexcept;
	void UpdateWaterLevelBelow(WorldLevel* pWorldLevel, WorldLevel* pWorldLevelBelow, const glm::ivec2& tile) const noexcept;

	void Evaporate(Scene* pScene, TileData * const * ppLevelData, std::vector<glm::ivec2>& toRemoveFloodingIDs, const glm::ivec2& tile, float dtS) const noexcept;

private:
	WorldLevel** m_ppLevels;
	uint32 m_NumLevels;
	std::vector<WorldObject> m_Objects;

	glm::ivec3* m_pStairs;
	uint32 m_NumStairs;

	std::vector<Room*> m_Rooms;
};

inline uint32 World::CanSpreadTo(const uint32 * const * ppLevel, const glm::ivec2& levelSize, const glm::ivec2& tileFrom, const glm::ivec2& tileTo) const noexcept
{
	if (tileTo.x <= 0 || tileTo.x >= levelSize.x - 1 || tileTo.y <= 0 || tileTo.y >= levelSize.y - 1)
	{
		return 0;
	}

	if (ppLevel[tileFrom.x][tileFrom.y] == ppLevel[tileTo.x][tileTo.y] || ppLevel[tileFrom.x][tileFrom.y] == 0 || ppLevel[tileTo.x][tileTo.y] == 0)
	{
		return 1;
	}

	return 0;
}

inline float World::CanFloodTo(const uint32 * const * ppLevel, const TileData * const * ppLevelData, const glm::ivec2& levelSize, const glm::ivec2& tileFrom, const glm::ivec2& tileTo, uint32 canSpreadTo) const noexcept
{
	if (canSpreadTo == 0)
	{
		return 0.0f;
	}

	if (ppLevelData[tileFrom.x][tileFrom.y].WaterLevel > ppLevelData[tileTo.x][tileTo.y].WaterLevel)
	{
		return ppLevelData[tileFrom.x][tileFrom.y].WaterLevel - ppLevelData[tileTo.x][tileTo.y].WaterLevel;
	}

	return 0.0f;
}

inline float World::CalculateFloodFactor(float waterLevelDifPosX, float waterLevelDifNegX, float waterLevelDifPosZ, float waterLevelDifNegZ, float dtS) const noexcept
{
	bool difPosXNotZero = waterLevelDifPosX > 0.0f;
	bool difNegXNotZero = waterLevelDifNegX > 0.0f;
	bool difPosZNotZero = waterLevelDifPosZ > 0.0f;
	bool difNegZNotZero = waterLevelDifNegZ > 0.0f;

	float denominator = 1.0f;
	float nominator = 0.0f;

	if (difPosXNotZero)
	{
		denominator += 1.0f;
		nominator = glm::max<float>(waterLevelDifPosX, nominator);
	}

	if (difNegXNotZero)
	{
		denominator += 1.0f;
		nominator = glm::max<float>(waterLevelDifNegX, nominator);
	}

	if (difPosZNotZero)
	{
		denominator += 1.0f;
		nominator = glm::max<float>(waterLevelDifPosZ, nominator);
	}

	if (difNegZNotZero)
	{
		denominator += 1.0f;
		nominator = glm::max<float>(waterLevelDifNegZ, nominator);
	}

	if (!(denominator > 0.0f))
	{
		return 0.0f;
	}

	float result = nominator / denominator;

	return glm::min(WATER_INV_TIME_FOR_WATER_TO_LEVEL * dtS * result, result);
}

inline void World::UpdateFloodingIds(TileData * const * ppLevelData, std::vector<glm::ivec2>& newFloodingIDs, const glm::ivec2& tilePos, uint32 canSpreadToo) const noexcept
{
	if (canSpreadToo == 1)
	{
		if (!ppLevelData[tilePos.x][tilePos.y].AlreadyFlooded)
		{
			newFloodingIDs.push_back(tilePos);
			ppLevelData[tilePos.x][tilePos.y].AlreadyFlooded = true;
		}
	}
}

inline void World::UpdateWaterLevel(TileData * const * ppLevelData, const glm::ivec2& tileFrom, const glm::ivec2& tileTo, float floodFactor, float waterLevelDif) const noexcept
{
	if (waterLevelDif > 0.0f)
	{
		ppLevelData[tileFrom.x][tileFrom.y].WaterLevelChange -= floodFactor;
		ppLevelData[tileTo.x][tileTo.y].WaterLevelChange += floodFactor;
	}
}

inline void World::UpdateFloodingIdsBelow(WorldLevel* pWorldLevelBelow, const glm::ivec2& tile) const noexcept
{
	if (!pWorldLevelBelow->GetLevelData()[tile.x][tile.y].AlreadyFlooded)
	{
		pWorldLevelBelow->GetFloodingIDs().push_back(tile);
		pWorldLevelBelow->GetLevelData()[tile.x][tile.y].AlreadyFlooded = true;
	}
}

inline void World::UpdateWaterLevelBelow(WorldLevel* pWorldLevel, WorldLevel* pWorldLevelBelow, const glm::ivec2& tile) const noexcept
{
	float missingWaterBelow = 1.0f - pWorldLevelBelow->GetLevelData()[tile.x][tile.y].WaterLevel;
	float waterLeft = glm::max<float>(pWorldLevel->GetLevelData()[tile.x][tile.y].WaterLevel - missingWaterBelow, 0.0f);
	pWorldLevelBelow->GetLevelData()[tile.x][tile.y].WaterLevelChange += pWorldLevel->GetLevelData()[tile.x][tile.y].WaterLevel - waterLeft;
	pWorldLevel->GetLevelData()[tile.x][tile.y].WaterLevelChange -= pWorldLevel->GetLevelData()[tile.x][tile.y].WaterLevel - waterLeft;
}

inline void World::Evaporate(Scene* scene, TileData * const * ppLevelData, std::vector<glm::ivec2>& toRemoveFloodingIDs, const glm::ivec2& tile, float dtS) const noexcept
{
	if (ppLevelData[tile.x][tile.y].WaterLevelAge < 0.01f)
	{
		ppLevelData[tile.x][tile.y].WaterLevel -= WATER_EVAPORATION_RATE * dtS;

		//std::cout << std::to_string(ppLevelData[tile.x][tile.y].WaterLevel) << std::endl;

		if (ppLevelData[tile.x][tile.y].WaterLevel < 0.0f)
		{
			ppLevelData[tile.x][tile.y].WaterLevel = 0.0f;
			ppLevelData[tile.x][tile.y].WaterLevelChange = 0.0f;
			ppLevelData[tile.x][tile.y].WaterLevelLastUpdated = 0.0f;
			ppLevelData[tile.x][tile.y].WaterLevelAge = 1.0f;
			ppLevelData[tile.x][tile.y].AlreadyFlooded = false;
			scene->GetGameObject(ppLevelData[tile.x][tile.y].WaterBlockName)->SetIsVisible(false);
			toRemoveFloodingIDs.push_back(tile);
		}
	}
}
