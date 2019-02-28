#include "../../Include/Scenarios/ScenarioWater.h"

ScenarioWater::ScenarioWater(bool waterAlwaysVisible)
{
	m_WaterAlwaysVisible = waterAlwaysVisible;
}

void ScenarioWater::Init(World* pWorld) noexcept
{
}

void ScenarioWater::OnStart(Scene* scene) noexcept
{

}

void ScenarioWater::OnEnd() noexcept
{

}

void ScenarioWater::OnVisibilityChange(World* pWorld, Scene* pScene, const std::vector<uint32>& activeRooms)
{
#if defined(PRINT_CPU_DEBUG_DATA)
	CPUProfiler::StartTimer(CPU_PROFILER_SLOT_2);
#endif
	for (uint32 levelIndex = 0; levelIndex < pWorld->GetNumLevels(); levelIndex += 2)
	{
		const uint32* const * ppLevel = pWorld->GetLevel(levelIndex)->GetLevel();
		TileData* const * ppLevelData = pWorld->GetLevel(levelIndex)->GetLevelData();
		std::vector<glm::ivec2>& floodingIDs = pWorld->GetLevel(levelIndex)->GetFloodingIDs();

		for (uint32 i = 0; i < floodingIDs.size(); i++)
		{
			glm::ivec2 currentTile = glm::ivec2(floodingIDs[i].x, floodingIDs[i].y);

			if (glm::abs(ppLevelData[currentTile.x][currentTile.y].WaterLevel > WATER_UPDATE_LEVEL_INTERVAL))
			{
				GameObject* pGameObject = ppLevelData[currentTile.x][currentTile.y].GameObjects[GAMEOBJECT_CONST_INDEX_WATER];
				bool waterIsVisible = m_WaterAlwaysVisible || std::find(activeRooms.begin(), activeRooms.end(), ppLevel[currentTile.x][currentTile.y]) != activeRooms.end();

				if (waterIsVisible)
				{
					pGameObject->SetIsVisible(true);
					pGameObject->UpdateTransform();
				}
				else
				{
					pGameObject->SetIsVisible(false);
				}
			}
		}
	}
#if defined(PRINT_CPU_DEBUG_DATA)
	CPUProfiler::EndTimer("Water On Visibility Change took %.3f ms", CPU_PROFILER_SLOT_2);
#endif
}

bool ScenarioWater::Update(float dtS, World* pWorld, Scene* pScene, const std::vector<uint32>& activeRooms) noexcept
{
#if defined(PRINT_CPU_DEBUG_DATA)
	CPUProfiler::StartTimer(CPU_PROFILER_SLOT_3);
#endif
	for (uint32 levelIndex = 0; levelIndex < pWorld->GetNumLevels(); levelIndex += 2)
	{
		const uint32* const * ppLevel = pWorld->GetLevel(levelIndex)->GetLevel();
		TileData* const * ppLevelData = pWorld->GetLevel(levelIndex)->GetLevelData();
		std::vector<glm::ivec2>& floodingIDs = pWorld->GetLevel(levelIndex)->GetFloodingIDs();
		glm::ivec2 levelSize = glm::ivec2(pWorld->GetLevel(levelIndex)->GetSizeX(), pWorld->GetLevel(levelIndex)->GetSizeZ());
		uint32 tilesBetweenBulkheads = pWorld->GetLevel(levelIndex)->GetTilesBetweenBulkheads();
		std::vector<glm::ivec2> newFloodingIDs;
		std::vector<glm::ivec2> toRemoveFloodingIDs;

		static glm::uvec2 spawnTile(10, 18);

		//TEMP
		if (levelIndex == 4 && Input::IsKeyDown(KEY_I))
		{
			ppLevelData[spawnTile.x][spawnTile.y].WaterLevel = 2.0f;
			ppLevelData[spawnTile.x][spawnTile.y].WaterLevelChange = 0.0f;
			ppLevelData[spawnTile.x][spawnTile.y].WaterLevelLastUpdated = 0.0f;
			ppLevelData[spawnTile.x][spawnTile.y].WaterLevelAge = 0.0f;
			ppLevelData[spawnTile.x][spawnTile.y].AlreadyFlooded = true;
			GameObject* pGameObject = ppLevelData[spawnTile.x][spawnTile.y].GameObjects[GAMEOBJECT_CONST_INDEX_WATER];
			//pGameObject->SetScale(glm::vec3(0.0f, ppLevelData[10][10].WaterLevelLastUpdated, 0.0f));
			//pGameObject->UpdateTransform();
			//pGameObject->SetIsVisible(true);

			bool containsSpawnTile = false;
			for (uint32 i = 0; i < floodingIDs.size(); i++)
			{
				if (floodingIDs[i].x == spawnTile.x && floodingIDs[i].y == spawnTile.y)
				{
					containsSpawnTile = true;
					break;
				}
			}

			if (!containsSpawnTile)
			{
				floodingIDs.push_back(glm::ivec2(spawnTile.x, spawnTile.y));
			}
		}

		//We are on the upper grid level of a world level

		
		for (uint32 i = 0; i < floodingIDs.size(); i++)
		{
			glm::ivec2 currentTile = glm::ivec2(floodingIDs[i].x, floodingIDs[i].y);

			if (ppLevelData[currentTile.x][currentTile.y].WaterLevel > ppLevelData[currentTile.x][currentTile.y].WaterLevelAge)
			{
				bool canFlowDown = false;
				if (levelIndex > 0)
				{
					if (pWorld->GetLevel(levelIndex - 2)->GetLevelData()[currentTile.x][currentTile.y].HasStairs)
					{
						if (ppLevelData[currentTile.x][currentTile.y].WaterLevel > WATER_UPDATE_LEVEL_INTERVAL)
						{
							UpdateFloodingIdsBelow(pWorld->GetLevel(levelIndex - 2), currentTile);
							canFlowDown = UpdateWaterLevelBelow(pWorld->GetLevel(levelIndex), pWorld->GetLevel(levelIndex - 2), currentTile);
						}
					}
				}

				//IF CANT FLOW DOWN, FLOW TO SIDE
				if (!canFlowDown)
				{
					ppLevelData[currentTile.x][currentTile.y].WaterLevelAge += ppLevelData[currentTile.x][currentTile.y].WaterLevel / WATER_AGING_DENOMINATOR;

					glm::ivec2 tilePosX = glm::ivec2(floodingIDs[i].x + 1, floodingIDs[i].y);
					glm::ivec2 tileNegX = glm::ivec2(floodingIDs[i].x - 1, floodingIDs[i].y);
					glm::ivec2 tilePosZ = glm::ivec2(floodingIDs[i].x, floodingIDs[i].y + 1);
					glm::ivec2 tileNegZ = glm::ivec2(floodingIDs[i].x, floodingIDs[i].y - 1);

					uint32 canSpreadToPosX = CanSpreadTo(ppLevel, levelSize, currentTile, tilePosX, ppLevelData);
					uint32 canSpreadToNegX = CanSpreadTo(ppLevel, levelSize, currentTile, tileNegX, ppLevelData);
					uint32 canSpreadToPosZ = CanSpreadTo(ppLevel, levelSize, currentTile, tilePosZ, ppLevelData);
					uint32 canSpreadToNegZ = CanSpreadTo(ppLevel, levelSize, currentTile, tileNegZ, ppLevelData);

					float waterLevelDifPosX = CanFloodTo(ppLevel, ppLevelData, currentTile, tilePosX, canSpreadToPosX);
					float waterLevelDifNegX = CanFloodTo(ppLevel, ppLevelData, currentTile, tileNegX, canSpreadToNegX);
					float waterLevelDifPosZ = CanFloodTo(ppLevel, ppLevelData, currentTile, tilePosZ, canSpreadToPosZ);
					float waterLevelDifNegZ = CanFloodTo(ppLevel, ppLevelData, currentTile, tileNegZ, canSpreadToNegZ);

					float floodDoorFactorPosX = CalculateDoorFloodFactor(ppLevel, ppLevelData, currentTile, tilePosX, tilesBetweenBulkheads, canSpreadToPosX);
					float floodDoorFactorNegX = CalculateDoorFloodFactor(ppLevel, ppLevelData, currentTile, tileNegX, tilesBetweenBulkheads, canSpreadToNegX);
					float floodDoorFactorPosZ = CalculateDoorFloodFactor(ppLevel, ppLevelData, currentTile, tilePosZ, tilesBetweenBulkheads, canSpreadToPosZ);
					float floodDoorFactorNegZ = CalculateDoorFloodFactor(ppLevel, ppLevelData, currentTile, tileNegZ, tilesBetweenBulkheads, canSpreadToNegZ);

					canSpreadToPosX *= (uint32)glm::ceil(floodDoorFactorPosX);
					canSpreadToNegX *= (uint32)glm::ceil(floodDoorFactorNegX);
					canSpreadToPosZ *= (uint32)glm::ceil(floodDoorFactorPosZ);
					canSpreadToNegZ *= (uint32)glm::ceil(floodDoorFactorNegZ);

					waterLevelDifPosX *= floodDoorFactorPosX;
					waterLevelDifNegX *= floodDoorFactorNegX;
					waterLevelDifPosZ *= floodDoorFactorPosZ;
					waterLevelDifNegZ *= floodDoorFactorNegZ;

					glm::vec4 floodFactor = CalculateFloodFactors(
						waterLevelDifPosX,
						waterLevelDifNegX,
						waterLevelDifPosZ,
						waterLevelDifNegZ,
						dtS);

					UpdateFloodingIds(ppLevelData, newFloodingIDs, tilePosX, canSpreadToPosX);
					UpdateFloodingIds(ppLevelData, newFloodingIDs, tileNegX, canSpreadToNegX);
					UpdateFloodingIds(ppLevelData, newFloodingIDs, tilePosZ, canSpreadToPosZ);
					UpdateFloodingIds(ppLevelData, newFloodingIDs, tileNegZ, canSpreadToNegZ);

					UpdateWaterLevel(ppLevelData, currentTile, tilePosX, floodFactor.x, waterLevelDifPosX);
					UpdateWaterLevel(ppLevelData, currentTile, tileNegX, floodFactor.y, waterLevelDifNegX);
					UpdateWaterLevel(ppLevelData, currentTile, tilePosZ, floodFactor.z, waterLevelDifPosZ);
					UpdateWaterLevel(ppLevelData, currentTile, tileNegZ, floodFactor.w, waterLevelDifNegZ);
				}
			}
			else
			{
				ppLevelData[currentTile.x][currentTile.y].WaterLevelAge /= 2.0f;
			}
		}

		for (uint32 i = 0; i < floodingIDs.size(); i++)
		{
			glm::ivec2 currentTile = glm::ivec2(floodingIDs[i].x, floodingIDs[i].y);

			float newActualWaterLevel = ppLevelData[currentTile.x][currentTile.y].WaterLevel + ppLevelData[currentTile.x][currentTile.y].WaterLevelChange;
			ppLevelData[currentTile.x][currentTile.y].WaterLevel = glm::min<float>(newActualWaterLevel, WATER_MAX_LEVEL);
			ppLevelData[currentTile.x][currentTile.y].WaterLevelChange = glm::max<float>(newActualWaterLevel - WATER_MAX_LEVEL, 0.0f);

			Evaporate(pScene, ppLevelData, toRemoveFloodingIDs, currentTile, dtS);

			GameObject* pGameObject = ppLevelData[currentTile.x][currentTile.y].GameObjects[GAMEOBJECT_CONST_INDEX_WATER];

			if (glm::abs(ppLevelData[currentTile.x][currentTile.y].WaterLevel - ppLevelData[currentTile.x][currentTile.y].WaterLevelLastUpdated) > WATER_UPDATE_LEVEL_INTERVAL)
			{
				ppLevelData[currentTile.x][currentTile.y].WaterLevelLastUpdated = glm::floor(WATER_ROUNDING_FACTOR * ppLevelData[currentTile.x][currentTile.y].WaterLevel) / WATER_ROUNDING_FACTOR;

				float yScale = glm::clamp(ppLevelData[currentTile.x][currentTile.y].WaterLevelLastUpdated, 0.05f, 1.90f);

				pGameObject->SetPosition(glm::vec3(currentTile.x, (float)levelIndex + 0.05f + yScale / 2.0f, currentTile.y));
				pGameObject->SetScale(glm::vec3(1.0f, yScale, 1.0f));

				bool waterIsVisible = m_WaterAlwaysVisible || std::find(activeRooms.begin(), activeRooms.end(), ppLevel[currentTile.x][currentTile.y]) != activeRooms.end();

				if (waterIsVisible)
				{
					pGameObject->SetIsVisible(true);
					pGameObject->UpdateTransform();
				}
				else
				{
					pGameObject->SetIsVisible(false);
				}
			}

		}

		floodingIDs.insert(floodingIDs.end(), newFloodingIDs.begin(), newFloodingIDs.end());
		newFloodingIDs.clear();

		for (uint32 i = 0; i < toRemoveFloodingIDs.size(); i++)
		{
			ppLevelData[toRemoveFloodingIDs[i].x][toRemoveFloodingIDs[i].y].WaterLevel = 0.0f;
			ppLevelData[toRemoveFloodingIDs[i].x][toRemoveFloodingIDs[i].y].WaterLevelChange = 0.0f;
			ppLevelData[toRemoveFloodingIDs[i].x][toRemoveFloodingIDs[i].y].WaterLevelLastUpdated = 0.0f;
			ppLevelData[toRemoveFloodingIDs[i].x][toRemoveFloodingIDs[i].y].WaterLevelAge = 1.0f;
			ppLevelData[toRemoveFloodingIDs[i].x][toRemoveFloodingIDs[i].y].AlreadyFlooded = false;
			ppLevelData[toRemoveFloodingIDs[i].x][toRemoveFloodingIDs[i].y].GameObjects[GAMEOBJECT_CONST_INDEX_WATER]->SetIsVisible(false);
			floodingIDs.erase(std::remove(floodingIDs.begin(), floodingIDs.end(), toRemoveFloodingIDs[i]));
		}

		toRemoveFloodingIDs.clear();

		//std::cout << floodingIDs.size() << std::endl;
	}

#if defined(PRINT_CPU_DEBUG_DATA)
	CPUProfiler::EndTimer("Water Scenario Update took %.3f ms", CPU_PROFILER_SLOT_3);
#endif

	return false;
}

std::string ScenarioWater::GetName() noexcept
{
	return "Vattenläcka";
}

int32 ScenarioWater::GetCooldownTime() noexcept
{
	return 0;
}

int32 ScenarioWater::GetMaxTimeBeforeOutbreak() noexcept
{
	return 1;
}