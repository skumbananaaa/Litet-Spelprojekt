#include "../../Include/Scenarios/ScenarioWater.h"
#include "../../Include/GameState.h"
#include "../../Include/Scenarios/ScenarioManager.h"

ScenarioWater::ScenarioWater(bool waterAlwaysVisible)
	: m_HasFlooded(false)
{
	m_WaterAlwaysVisible = waterAlwaysVisible;
}

ScenarioWater::~ScenarioWater()
{
	DeleteArrSafe(m_FloodingIDs);
}

void ScenarioWater::BeginReplay(SceneGame* pScene, void* userData) noexcept
{
	auto pair = (std::pair<glm::ivec3, float>*)userData;
	StartWater(pair->first, pair->second);
	ScenarioManager::StartScenario(this);
}

void ScenarioWater::DestroyUserData(void *& userData) noexcept
{
	std::pair<glm::vec3, float>* pair = reinterpret_cast<std::pair<glm::vec3, float>*>(userData);
	DeleteSafe(pair);
	userData = pair;
}

void ScenarioWater::Init(World* pWorld) noexcept
{
	SetTimeOfNextOutBreak(1.0f);
	m_FloodingIDs = new std::vector<glm::ivec2>[pWorld->GetNumLevels() / 2];
	m_pWorld = pWorld;
}

void ScenarioWater::Release() noexcept
{
	m_InletTiles.clear();
	m_WaterIntakeRates.clear();
	m_HasFlooded = false;
	DeleteArrSafe(m_FloodingIDs);
}

void ScenarioWater::OnStart(SceneGame* scene) noexcept
{
}

void ScenarioWater::OnEnd(SceneGame* scene) noexcept
{
}

void ScenarioWater::Escalate(const glm::ivec3& position, float severity) noexcept
{
	if (!IsReplaying())
	{
		StartWater(position, severity);
		RegisterReplayEvent(new std::pair<glm::ivec3, float>(position, severity));
	}
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
				Room& room = pWorld->GetRoom(ppLevel[currentTile.x][currentTile.y]);
				bool waterIsVisible = m_WaterAlwaysVisible || std::find(activeRooms.begin(), activeRooms.end(), ppLevel[currentTile.x][currentTile.y]) != activeRooms.end() || room.IsFloodDetected();

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
	m_TotalWaterLevel = 0.0f;

	const std::vector<uint32>& activeRooms = pWorld->GetActiveRooms();
#if defined(PRINT_CPU_DEBUG_DATA)
	CPUProfiler::StartTimer(CPU_PROFILER_SLOT_3);
#endif

	for (int i = 0; i < m_InletTiles.size(); i++)
	{
		TileData* const * ppLevelData = pWorld->GetLevel(m_InletTiles[i].y).GetLevelData();
		TileData& tile = ppLevelData[m_InletTiles[i].x][m_InletTiles[i].z];
		tile.WaterLevel += m_WaterIntakeRates[i] * dtS;
		tile.WaterLevel = std::min(tile.WaterLevel, 2.0f);
		tile.WaterLevelChange = 0.0f;
		tile.WaterLevelLastUpdated = 0.0f;
		tile.WaterLevelAge = 0.0f;
		tile.AlreadyFlooded = true;

		if (!tile.WaterInlet)
		{
			m_InletsToRemove.push_back(i);
		}

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

					UpdateWaterLevel(ppLevelData, currentTile, tilePosX, floodFactor.x, canSpreadToPosX);
					UpdateWaterLevel(ppLevelData, currentTile, tileNegX, floodFactor.y, canSpreadToNegX);
					UpdateWaterLevel(ppLevelData, currentTile, tilePosZ, floodFactor.z, canSpreadToPosZ);
					UpdateWaterLevel(ppLevelData, currentTile, tileNegZ, floodFactor.w, canSpreadToNegZ);
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

			Evaporate(pScene, ppLevelData, ppLevel, toRemoveFloodingIDs, currentTile, dtS);
			ExtinguishFire(ppLevelData, pWorld->GetLevel(levelIndex + 1).GetLevelData(), currentTile, dtS);

			GameObject* pGameObject = ppLevelData[currentTile.x][currentTile.y].GameObjects[GAMEOBJECT_CONST_INDEX_WATER];

			float waterlevel = ppLevelData[currentTile.x][currentTile.y].WaterLevel;
			m_TotalWaterLevel += (waterlevel / WATER_MAX_LEVEL);

			if (glm::abs(waterlevel - ppLevelData[currentTile.x][currentTile.y].WaterLevelLastUpdated) > WATER_UPDATE_LEVEL_INTERVAL)
			{
				ppLevelData[currentTile.x][currentTile.y].WaterLevelLastUpdated = glm::floor(WATER_ROUNDING_FACTOR * waterlevel) / WATER_ROUNDING_FACTOR;

				float yScale = glm::clamp(ppLevelData[currentTile.x][currentTile.y].WaterLevelLastUpdated, 0.05f, 1.90f);

				pGameObject->SetPosition(glm::vec3(currentTile.x, (float)levelIndex + 0.05f + yScale / 2.0f, currentTile.y));
				pGameObject->SetScale(glm::vec3(1.0f, yScale, 1.0f));

				Room& room = pWorld->GetRoom(ppLevel[currentTile.x][currentTile.y]);
				bool waterIsVisible = m_WaterAlwaysVisible || std::find(activeRooms.begin(), activeRooms.end(), ppLevel[currentTile.x][currentTile.y]) != activeRooms.end() || room.IsFloodDetected();

				if (!room.IsFlooded() && !room.IsFloodUpdatedThisFrame())
				{
					room.SetFlooded(true);
					room.SetFloodUpdated(true);
				}

				if (waterIsVisible)
				{
					if (!pGameObject->IsVisible())
					{
						OnVisibilityChange(pWorld, pScene);
					}
					pGameObject->SetIsVisible(true);
					pGameObject->UpdateTransform();
				}
				else
				{
					if (pGameObject->IsVisible())
					{
						OnVisibilityChange(pWorld, pScene);
					}
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
	for (uint32 i = 0; i < m_InletsToRemove.size(); i++)
	{
		m_InletTiles.erase(m_InletTiles.begin() + m_InletsToRemove[i] - i);
	}

	m_InletsToRemove.clear();

	constexpr float total = 40.0f * 10.0f;
	GameState::SetWaterLeakAmount(m_TotalWaterLevel / total);

	//Will work for now since the first level is the only one that gets flooded
	return m_HasFlooded && m_FloodingIDs[0].empty();
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

bool ScenarioWater::IsComplete() noexcept
{
	//Will work for now since the first level is the only one that gets flooded
	return m_HasFlooded && m_FloodingIDs[0].empty();
}

const std::vector<glm::ivec3> ScenarioWater::GetWaterInlets() const noexcept
{
	return m_InletTiles;
}
