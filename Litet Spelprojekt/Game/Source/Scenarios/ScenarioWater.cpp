#include "../../Include/Scenarios/ScenarioWater.h"

ScenarioWater::ScenarioWater(bool waterAlwaysVisible)
{
	m_WaterAlwaysVisible = waterAlwaysVisible;
}

ScenarioWater::~ScenarioWater()
{
	DeleteArrSafe(m_FloodingIDs);
}

void ScenarioWater::Init(World* pWorld) noexcept
{
	m_FloodingIDs = new std::vector<glm::ivec2>[pWorld->GetNumLevels() / 2];
}

void ScenarioWater::OnStart(SceneGame* scene) noexcept
{
	
}

void ScenarioWater::OnEnd(SceneGame* scene) noexcept
{
	DeleteArrSafe(m_FloodingIDs);
}

void ScenarioWater::Escalate(const glm::ivec3& position) noexcept
{
	m_InletTiles.push_back(position);
	m_FloodingIDs[position.y / 2].push_back(glm::ivec2(position.x, position.z));
}

void ScenarioWater::OnVisibilityChange(World* pWorld, SceneGame* pScene)
{
	const std::vector<uint32>& activeRooms = pWorld->GetActiveRooms();

#if defined(PRINT_CPU_DEBUG_DATA)
	CPUProfiler::StartTimer(CPU_PROFILER_SLOT_2);
#endif
	for (uint32 levelIndex = 0; levelIndex < pWorld->GetNumLevels(); levelIndex += 2)
	{
		const uint32* const * ppLevel = pWorld->GetLevel(levelIndex).GetLevel();
		TileData* const * ppLevelData = pWorld->GetLevel(levelIndex).GetLevelData();
		std::vector<glm::ivec2>& floodingIDs = m_FloodingIDs[levelIndex / 2];

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

bool ScenarioWater::Update(float dtS, World* pWorld, SceneGame* pScene) noexcept
{
	const std::vector<uint32>& activeRooms = pWorld->GetActiveRooms();

#if defined(PRINT_CPU_DEBUG_DATA)
	CPUProfiler::StartTimer(CPU_PROFILER_SLOT_3);
#endif

	for (int i = 0; i < m_InletTiles.size(); i++)
	{
		TileData* const * ppLevelData = pWorld->GetLevel(m_InletTiles[i].y).GetLevelData();
		ppLevelData[m_InletTiles[i].x][m_InletTiles[i].z].WaterLevel = 2.0f;
		ppLevelData[m_InletTiles[i].x][m_InletTiles[i].z].WaterLevelChange = 0.0f;
		ppLevelData[m_InletTiles[i].x][m_InletTiles[i].z].WaterLevelLastUpdated = 0.0f;
		ppLevelData[m_InletTiles[i].x][m_InletTiles[i].z].WaterLevelAge = 0.0f;
		ppLevelData[m_InletTiles[i].x][m_InletTiles[i].z].AlreadyFlooded = true;
		GameObject* pGameObject = ppLevelData[m_InletTiles[i].x][m_InletTiles[i].z].GameObjects[GAMEOBJECT_CONST_INDEX_WATER];

		/*bool containsSpawnTile = false;
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
		}*/
	}

	for (uint32 levelIndex = 0; levelIndex < pWorld->GetNumLevels(); levelIndex += 2)
	{
		const uint32* const * ppLevel = pWorld->GetLevel(levelIndex).GetLevel();
		TileData* const * ppLevelData = pWorld->GetLevel(levelIndex).GetLevelData();
		std::vector<glm::ivec2>& floodingIDs = m_FloodingIDs[levelIndex / 2];
		glm::ivec2 levelSize = glm::ivec2(pWorld->GetLevel(levelIndex).GetSizeX(), pWorld->GetLevel(levelIndex).GetSizeZ());
		uint32 tilesBetweenBulkheads = pWorld->GetLevel(levelIndex).GetTilesBetweenBulkheads();
		std::vector<glm::ivec2> newFloodingIDs;
		std::vector<glm::ivec2> toRemoveFloodingIDs;

		//We are on the upper grid level of a world level

		
		for (uint32 i = 0; i < floodingIDs.size(); i++)
		{
			glm::ivec2 currentTile = glm::ivec2(floodingIDs[i].x, floodingIDs[i].y);

			if (ppLevelData[currentTile.x][currentTile.y].WaterLevel > ppLevelData[currentTile.x][currentTile.y].WaterLevelAge)
			{
				bool canFlowDown = false;
				if (levelIndex > 0)
				{
					if (pWorld->GetLevel(levelIndex - 2).GetLevelData()[currentTile.x][currentTile.y].HasStairs)
					{
						if (ppLevelData[currentTile.x][currentTile.y].WaterLevel > WATER_UPDATE_LEVEL_INTERVAL)
						{
							UpdateFloodingIdsBelow(pWorld->GetLevel(levelIndex - 2), levelIndex - 1, currentTile);
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
			ExtinguishFire(ppLevelData, pWorld->GetLevel(levelIndex + 1).GetLevelData(), currentTile, dtS);

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