#pragma once
#include <World/Scenarios/IScenario.h>
#include <World/GameObjectDoor.h>

#if defined(PRINT_CPU_DEBUG_DATA)
#include <System/CPUProfiler.h>
#endif

constexpr float WATER_UPDATE_LEVEL_INTERVAL = 0.02f;
constexpr float WATER_ROUNDING_FACTOR = 50.0f;
constexpr float WATER_EVAPORATION_RATE = 1.0f / 1000.0f;
constexpr float WATER_AGING_DENOMINATOR = 1.0f;
constexpr float WATER_INV_TIME_FOR_WATER_TO_LEVEL = 30.0f;
constexpr float WATER_MAX_LEVEL = 2.0f;

class ScenarioWater : public IScenario
{
public:
	ScenarioWater(bool waterAlwaysVisible = false);

	virtual void Init(World* pWorld) noexcept override;
	virtual void OnStart(Scene* scene) noexcept override;
	virtual void OnEnd()noexcept override;
	virtual void OnVisibilityChange(World* pWorld, Scene* pScene, const std::vector<uint32>& activeRooms) override;
	virtual bool Update(float dtS, World* pWorld, Scene* pScene,  const std::vector<uint32>& activeRooms) noexcept override;
	virtual std::string GetName() noexcept override;
	virtual int32 GetCooldownTime() noexcept override;
	virtual int32 GetMaxTimeBeforeOutbreak() noexcept override;

private:
	bool m_WaterAlwaysVisible;

private:
	//Scenario Spread Helper Functions
	uint32 CanSpreadTo(const uint32 * const * ppLevel, const glm::ivec2& levelSize, const glm::ivec2& tileFrom, const glm::ivec2& tileTo, const TileData* const* ppLevelData) const noexcept;

	//Water Scenario Helper Functions
	float CanFloodTo(const uint32 * const * ppLevel, const TileData * const * ppLevelData, const glm::ivec2& tileFrom, const glm::ivec2& tileTo, uint32 canSpreadTo) const noexcept;
	float CalculateDoorFloodFactor(const uint32 * const * ppLevel, const TileData * const * ppLevelData, const glm::ivec2& tileFrom, const glm::ivec2& tileTo, uint32 tilesBetweenBulkheads, uint32 canSpreadTo) const noexcept;
	glm::vec4 CalculateFloodFactors(float waterLevelDifPosX, float waterLevelDifNegX, float waterLevelDifPosZ, float waterLevelDifNegZ, float dtS) const noexcept;

	void UpdateFloodingIds(TileData * const * ppLevelData, std::vector<glm::ivec2>& newFloodingIDs, const glm::ivec2& tilePos, uint32 canSpreadToo) const noexcept;
	void UpdateWaterLevel(TileData * const * ppLevelData, const glm::ivec2& tileFrom, const glm::ivec2& tileTo, float floodFactor, float waterLevelDif) const noexcept;

	void UpdateFloodingIdsBelow(WorldLevel* pWorldLevel, const glm::ivec2& tile) const noexcept;
	bool UpdateWaterLevelBelow(WorldLevel* pWorldLevel, WorldLevel* pWorldLevelBelow, const glm::ivec2& tile) const noexcept;

	void Evaporate(Scene* pScene, TileData * const * ppLevelData, std::vector<glm::ivec2>& toRemoveFloodingIDs, const glm::ivec2& tile, float dtS) const noexcept;
};

inline uint32 ScenarioWater::CanSpreadTo(const uint32 * const * ppLevel, const glm::ivec2& levelSize, const glm::ivec2& tileFrom, const glm::ivec2& tileTo, const TileData* const* ppLevelData) const noexcept
{
	if (tileTo.x <= 0 || tileTo.x >= levelSize.x - 1 || tileTo.y <= 0 || tileTo.y >= levelSize.y - 1)
	{
		return 0;
	}

	//if (ppLevel[tileFrom.x][tileFrom.y] == ppLevel[tileTo.x][tileTo.y] || ppLevelData[tileFrom.x][tileFrom.y].HasDoor() || ppLevelData[tileTo.x][tileTo.y].HasDoor())
	if (ppLevel[tileFrom.x][tileFrom.y] == ppLevel[tileTo.x][tileTo.y] || (ppLevelData[tileFrom.x][tileFrom.y].HasDoor() && ppLevelData[tileTo.x][tileTo.y].HasDoor()))
	{
		return 1;
	}

	return 0;
}

inline float ScenarioWater::CanFloodTo(const uint32 * const * ppLevel, const TileData * const * ppLevelData, const glm::ivec2& tileFrom, const glm::ivec2& tileTo, uint32 canSpreadTo) const noexcept
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

inline float ScenarioWater::CalculateDoorFloodFactor(const uint32 * const * ppLevel, const TileData * const * ppLevelData, const glm::ivec2& tileFrom, const glm::ivec2& tileTo, uint32 tilesBetweenBulkheads, uint32 canSpreadTo) const noexcept
{
	if (canSpreadTo == 0)
	{
		return 0.0f;
	}

	if (!ppLevelData[tileTo.x][tileTo.y].HasDoor())
	{
		return 1.0f;
	}

	//Water is trying to flood to a different room
	if (ppLevel[tileFrom.x][tileFrom.y] != ppLevel[tileTo.x][tileTo.y])
	{
		bool doorIsOpen = !reinterpret_cast<GameObjectDoor*>(ppLevelData[tileTo.x][tileTo.y].GameObjects[GAMEOBJECT_CONST_INDEX_DOOR])->IsClosed();

		//If the smallest of the tiles x coordinates is a multiple of "tilesBetweenBulkheads", the water is trying to flood over a bulkhead.
		//Since CanSpreadTo returns 0 if there isnt a door when the water is trying to flood to a different room we know its trying to flood over a door.
		if (glm::min(tileFrom.y, tileTo.y) % tilesBetweenBulkheads == 0)
		{
			return doorIsOpen ? 1.0f : 0.0f;
		}

		//If the water is trying to flood over a door that is not in a bulkhead, reduce the flood factor.
		return doorIsOpen ? 1.0f : 0.25f;
	}

	//Water is not trying to flood to a different room but tileTo has a door.
	return 1.0f;
}

inline glm::vec4 ScenarioWater::CalculateFloodFactors(
	float waterLevelDifPosX, float waterLevelDifNegX, float waterLevelDifPosZ, float waterLevelDifNegZ, float dtS) const noexcept
{
	bool difPosXNotZero = waterLevelDifPosX > 0.0f;
	bool difNegXNotZero = waterLevelDifNegX > 0.0f;
	bool difPosZNotZero = waterLevelDifPosZ > 0.0f;
	bool difNegZNotZero = waterLevelDifNegZ > 0.0f;

	float denominator = 1.0f;
	glm::vec4 result(0.0f);

	if (difPosXNotZero)
	{
		denominator += 1.0f;
		result.x = WATER_INV_TIME_FOR_WATER_TO_LEVEL * dtS * waterLevelDifPosX;
	}

	if (difNegXNotZero)
	{
		denominator += 1.0f;
		result.y = WATER_INV_TIME_FOR_WATER_TO_LEVEL * dtS * waterLevelDifNegX;
	}

	if (difPosZNotZero)
	{
		denominator += 1.0f;
		result.z = WATER_INV_TIME_FOR_WATER_TO_LEVEL * dtS * waterLevelDifPosZ;
	}

	if (difNegZNotZero)
	{
		denominator += 1.0f;
		result.w = WATER_INV_TIME_FOR_WATER_TO_LEVEL * dtS * waterLevelDifNegZ;
	}

	if (!(denominator > 0.0f))
	{
		return glm::vec4(0.0f);
	}

	return result / denominator;
}

inline void ScenarioWater::UpdateFloodingIds(TileData * const * ppLevelData, std::vector<glm::ivec2>& newFloodingIDs, const glm::ivec2& tilePos, uint32 canSpreadToo) const noexcept
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

inline void ScenarioWater::UpdateWaterLevel(TileData * const * ppLevelData, const glm::ivec2& tileFrom, const glm::ivec2& tileTo, float floodFactor, float waterLevelDif) const noexcept
{
	//if (waterLevelDif > 0.0f)
	{
		ppLevelData[tileFrom.x][tileFrom.y].WaterLevelChange -= floodFactor;
		ppLevelData[tileTo.x][tileTo.y].WaterLevelChange += floodFactor;
	}
}

inline void ScenarioWater::UpdateFloodingIdsBelow(WorldLevel* pWorldLevelBelow, const glm::ivec2& tile) const noexcept
{
	if (!pWorldLevelBelow->GetLevelData()[tile.x][tile.y].AlreadyFlooded)
	{
		pWorldLevelBelow->GetFloodingIDs().push_back(tile);
		pWorldLevelBelow->GetLevelData()[tile.x][tile.y].AlreadyFlooded = true;
	}
}

inline bool ScenarioWater::UpdateWaterLevelBelow(WorldLevel* pWorldLevel, WorldLevel* pWorldLevelBelow, const glm::ivec2& tile) const noexcept
{
	float missingWaterBelow = WATER_MAX_LEVEL - pWorldLevelBelow->GetLevelData()[tile.x][tile.y].WaterLevel;
	float waterLeft = glm::max<float>(pWorldLevel->GetLevelData()[tile.x][tile.y].WaterLevel - missingWaterBelow, 0.0f);
	float waterLevelChange = pWorldLevel->GetLevelData()[tile.x][tile.y].WaterLevel - waterLeft;
	pWorldLevelBelow->GetLevelData()[tile.x][tile.y].WaterLevelChange += waterLevelChange;
	pWorldLevel->GetLevelData()[tile.x][tile.y].WaterLevelChange -= waterLevelChange;

	if (waterLevelChange > 0.0f)
	{
		return true;
	}

	return false;
}

inline void ScenarioWater::Evaporate(Scene* scene, TileData * const * ppLevelData, std::vector<glm::ivec2>& toRemoveFloodingIDs, const glm::ivec2& tile, float dtS) const noexcept
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
			ppLevelData[tile.x][tile.y].GameObjects[GAMEOBJECT_CONST_INDEX_WATER]->SetIsVisible(false);
			toRemoveFloodingIDs.push_back(tile);
		}
	}
}
