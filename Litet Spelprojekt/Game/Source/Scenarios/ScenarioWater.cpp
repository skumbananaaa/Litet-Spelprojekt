#include "../../Include/Scenarios/ScenarioWater.h"

ScenarioWater::ScenarioWater()
{
}

void ScenarioWater::OnStart() noexcept
{

}

void ScenarioWater::OnEnd() noexcept
{

}

bool ScenarioWater::Update(float dtS, World* pWorld, Scene* pScene, const std::vector<uint32>& activeRooms) noexcept
{
	for (uint32 levelIndex = 0; levelIndex < pWorld->GetNumLevels(); levelIndex++)
	{
		const uint32* const * ppLevel = pWorld->GetLevel(levelIndex)->GetLevel();
		TileData* const * ppLevelData = pWorld->GetLevel(levelIndex)->GetLevelData();
		std::vector<glm::ivec2>& floodingIDs = pWorld->GetLevel(levelIndex)->GetFloodingIDs();
		glm::ivec2 levelSize = glm::ivec2(pWorld->GetLevel(levelIndex)->GetSizeX(), pWorld->GetLevel(levelIndex)->GetSizeZ());
		std::vector<glm::ivec2> newFloodingIDs;
		std::vector<glm::ivec2> toRemoveFloodingIDs;

		//TEMP
		if (levelIndex == 5 && Input::IsKeyDown(KEY_I))
		{
			ppLevelData[10][10].WaterLevel = 1.0f;
			ppLevelData[10][10].WaterLevelAge = 0.0f;
			ppLevelData[10][10].AlreadyFlooded = true;
			GameObject* pGameObject = pScene->GetGameObject(ppLevelData[10][10].WaterBlockName);
			pGameObject->SetScale(glm::vec3(0.0f, ppLevelData[10][10].WaterLevelLastUpdated, 0.0f));
			//pGameObject->UpdateTransform();
			//pGameObject->SetIsVisible(true);

			bool contains1010 = false;
			for (uint32 i = 0; i < floodingIDs.size(); i++)
			{
				if (floodingIDs[i].x == 10 && floodingIDs[i].y == 10)
				{
					contains1010 = true;
					break;
				}
			}

			if (!contains1010)
			{
				floodingIDs.push_back(glm::ivec2(10, 10));
			}
		}

		bool isOnUpperGridLevel = levelIndex % 2 == 1;

		//We are on the upper grid level of a world level

		
		for (uint32 i = 0; i < floodingIDs.size(); i++)
		{
			glm::ivec2 currentTile = glm::ivec2(floodingIDs[i].x, floodingIDs[i].y);
			bool canFlowDown = false;

			//IF ON UPPER GRID LEVEL ON BOAT LEVEL, TRY TO FLOW DOWN
			if (isOnUpperGridLevel)
			{
				if (ppLevelData[currentTile.x][currentTile.y].WaterLevel > WATER_UPDATE_INTERVAL)
				{
					UpdateFloodingIdsBelow(pWorld->GetLevel(levelIndex - 1), currentTile);
					canFlowDown = UpdateWaterLevelBelow(pWorld->GetLevel(levelIndex), pWorld->GetLevel(levelIndex - 1), currentTile);
				}
			}
			//IF ON LOWER GRID LEVEL ON BOAT LEVEL, CHECK FOR STAIRS
			else
			{
				if (levelIndex > 0)
				{
					if (pWorld->GetLevel(levelIndex - 2)->GetLevelData()[currentTile.x][currentTile.y].HasStairs)
					{
						if (ppLevelData[currentTile.x][currentTile.y].WaterLevel > WATER_UPDATE_INTERVAL)
						{
							UpdateFloodingIdsBelow(pWorld->GetLevel(levelIndex - 1), currentTile);
							canFlowDown = UpdateWaterLevelBelow(pWorld->GetLevel(levelIndex), pWorld->GetLevel(levelIndex - 1), currentTile);
						}
					}
				}
			}

			//IF CANT FLOW DOWN, FLOW TO SIDE
			if (!canFlowDown)
			{
				if (ppLevelData[currentTile.x][currentTile.y].WaterLevel > ppLevelData[currentTile.x][currentTile.y].WaterLevelAge)
				{
					ppLevelData[currentTile.x][currentTile.y].WaterLevelAge += ppLevelData[currentTile.x][currentTile.y].WaterLevel / WATER_AGING_DENOMINATOR;

					glm::ivec2 tilePosX = glm::ivec2(floodingIDs[i].x + 1, floodingIDs[i].y);
					glm::ivec2 tileNegX = glm::ivec2(floodingIDs[i].x - 1, floodingIDs[i].y);
					glm::ivec2 tilePosZ = glm::ivec2(floodingIDs[i].x, floodingIDs[i].y + 1);
					glm::ivec2 tileNegZ = glm::ivec2(floodingIDs[i].x, floodingIDs[i].y - 1);

					uint32 canSpreadToPosX = CanSpreadTo(ppLevel, levelSize, currentTile, tilePosX);
					uint32 canSpreadToNegX = CanSpreadTo(ppLevel, levelSize, currentTile, tileNegX);
					uint32 canSpreadToPosZ = CanSpreadTo(ppLevel, levelSize, currentTile, tilePosZ);
					uint32 canSpreadToNegZ = CanSpreadTo(ppLevel, levelSize, currentTile, tileNegZ);

					float waterLevelDifPosX = CanFloodTo(ppLevel, ppLevelData, levelSize, currentTile, tilePosX, canSpreadToPosX);
					float waterLevelDifNegX = CanFloodTo(ppLevel, ppLevelData, levelSize, currentTile, tileNegX, canSpreadToNegX);
					float waterLevelDifPosZ = CanFloodTo(ppLevel, ppLevelData, levelSize, currentTile, tilePosZ, canSpreadToPosZ);
					float waterLevelDifNegZ = CanFloodTo(ppLevel, ppLevelData, levelSize, currentTile, tileNegZ, canSpreadToNegZ);

					float floodFactor = CalculateFloodFactor(waterLevelDifPosX, waterLevelDifNegX, waterLevelDifPosZ, waterLevelDifNegZ, dtS);

					UpdateFloodingIds(ppLevelData, newFloodingIDs, tilePosX, canSpreadToPosX);
					UpdateFloodingIds(ppLevelData, newFloodingIDs, tileNegX, canSpreadToNegX);
					UpdateFloodingIds(ppLevelData, newFloodingIDs, tilePosZ, canSpreadToPosZ);
					UpdateFloodingIds(ppLevelData, newFloodingIDs, tileNegZ, canSpreadToNegZ);

					UpdateWaterLevel(ppLevelData, currentTile, tilePosX, floodFactor, waterLevelDifPosX);
					UpdateWaterLevel(ppLevelData, currentTile, tileNegX, floodFactor, waterLevelDifNegX);
					UpdateWaterLevel(ppLevelData, currentTile, tilePosZ, floodFactor, waterLevelDifPosZ);
					UpdateWaterLevel(ppLevelData, currentTile, tileNegZ, floodFactor, waterLevelDifNegZ);
				}
				else
				{
					ppLevelData[currentTile.x][currentTile.y].WaterLevelAge /= 2.0f;
				}
			}
		}

		for (uint32 i = 0; i < floodingIDs.size(); i++)
		{
			glm::ivec2 currentTile = glm::ivec2(floodingIDs[i].x, floodingIDs[i].y);

			ppLevelData[currentTile.x][currentTile.y].WaterLevel = glm::min<float>(ppLevelData[currentTile.x][currentTile.y].WaterLevel + ppLevelData[currentTile.x][currentTile.y].WaterLevelChange, 1.0f);
			ppLevelData[currentTile.x][currentTile.y].WaterLevelChange = 0.0f;

			Evaporate(pScene, ppLevelData, toRemoveFloodingIDs, currentTile, dtS);

			GameObject* pGameObject = pScene->GetGameObject(ppLevelData[currentTile.x][currentTile.y].WaterBlockName);
			bool waterIsVisible = std::find(activeRooms.begin(), activeRooms.end(), ppLevel[currentTile.x][currentTile.y]) != activeRooms.end();
			pGameObject->SetIsVisible(waterIsVisible);

			if (glm::abs(ppLevelData[currentTile.x][currentTile.y].WaterLevel - ppLevelData[currentTile.x][currentTile.y].WaterLevelLastUpdated) > WATER_UPDATE_INTERVAL)
			{
				ppLevelData[currentTile.x][currentTile.y].WaterLevelLastUpdated = (float)((uint32)(WATER_ROUNDING_FACTOR * ppLevelData[currentTile.x][currentTile.y].WaterLevel)) / WATER_ROUNDING_FACTOR;

				if (isOnUpperGridLevel)
				{
					TileData * const * ppLevelDataBelow = pWorld->GetLevel(levelIndex - 1)->GetLevelData();

					if (ppLevelData[currentTile.x][currentTile.y].WaterFloodsFromeAbove)
					{
						float xzScale = glm::max(ppLevelDataBelow[currentTile.x][currentTile.y].WaterLevelLastUpdated, 0.05f);
						pGameObject->SetPosition(glm::vec3(currentTile.x, (float)levelIndex, currentTile.y));
						pGameObject->SetScale(glm::vec3(xzScale, 2.0f, xzScale));
					}
					else
					{
						GameObject* pGameObjectBelow = pScene->GetGameObject(ppLevelDataBelow[currentTile.x][currentTile.y].WaterBlockName);

						float yScale = glm::max(ppLevelData[currentTile.x][currentTile.y].WaterLevelLastUpdated, 0.05f);
						float yPos = pGameObjectBelow->GetPosition().y + (pGameObjectBelow->GetScale().y + yScale) / 2.0f;
						pGameObject->SetPosition(glm::vec3(currentTile.x, yPos, currentTile.y));
						pGameObject->SetScale(glm::vec3(1.0f, yScale, 1.0f));
					}
				}
				else
				{
					float yScale = glm::max(ppLevelData[currentTile.x][currentTile.y].WaterLevelLastUpdated, 0.05f);
					pGameObject->SetPosition(glm::vec3(currentTile.x, (float)levelIndex + 0.05f + ppLevelData[currentTile.x][currentTile.y].WaterLevelLastUpdated / 2.0f, currentTile.y));
					pGameObject->SetScale(glm::vec3(1.0f, yScale, 1.0f));
				}

				pGameObject->UpdateTransform();

				ppLevelData[currentTile.x][currentTile.y].WaterFloodsFromeAbove = false;
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
			pScene->GetGameObject(ppLevelData[toRemoveFloodingIDs[i].x][toRemoveFloodingIDs[i].y].WaterBlockName)->SetIsVisible(false);
			floodingIDs.erase(std::remove(floodingIDs.begin(), floodingIDs.end(), toRemoveFloodingIDs[i]));
		}

		toRemoveFloodingIDs.clear();

		//std::cout << floodingIDs.size() << std::endl;
	}

	return false;
}

std::string ScenarioWater::GetName() noexcept
{
	return "Water";
}

int32 ScenarioWater::GetCooldownTime() noexcept
{
	return 0;
}

int32 ScenarioWater::GetMaxTimeBeforeOutbreak() noexcept
{
	return 1;
}