#include <EnginePch.h>
#include <World/World.h>

World::World(WorldLevel* worldLevels[], uint32 numLevels, WorldObject* objects, uint32 numObjects) noexcept
{
	m_NumLevels = numLevels;
	m_ppLevels = new WorldLevel*[m_NumLevels];

	for (uint32 i = 0; i < m_NumLevels; i++)
	{
		m_ppLevels[i] = worldLevels[i];
	}

	for (uint32 i = 0; i < numObjects; i++)
	{
		m_Objects.push_back(objects[i]);
	}

	m_pStairs = nullptr;
	m_NumStairs = 0;
}

World::~World()
{
	for (uint32 i = 0; i < m_NumLevels; i++)
	{
		delete m_ppLevels[i];
		m_ppLevels[i] = nullptr;
	}

	DeleteArrSafe(m_ppLevels);
	DeleteArrSafe(m_pStairs);

	for (size_t i = 0; i < m_Rooms.size(); i++)
	{
		DeleteSafe(m_Rooms[i]);
	}
}

void World::AddWorldObject(const WorldObject& object) noexcept
{
	m_Objects.push_back(object);
}

const WorldLevel* const World::GetLevel(uint32 level) const noexcept
{
	assert(level < m_NumLevels);
	return m_ppLevels[level];
}

uint32 World::GetNumLevels() const noexcept
{
	return m_NumLevels;
}

const WorldObject& World::GetWorldObject(uint32 index) const noexcept
{
	assert(index < m_Objects.size());
	return m_Objects[index];
}

uint32 World::GetNumWorldObjects() const noexcept
{
	return static_cast<uint32>(m_Objects.size());
}

void World::GenerateRooms()
{
	std::vector<glm::uvec4> m_RoomBounds;
	std::vector<glm::uvec4> temp;

	std::vector<glm::vec3> center;

	for (int level = 0; level < m_NumLevels; level += 2)
	{
		m_ppLevels[level]->GenerateRooms();
		temp = m_ppLevels[level]->GetRooms();
		for (size_t i = 0; i < temp.size(); i++)
		{
			if (i >= m_RoomBounds.size())
			{
				m_RoomBounds.push_back(temp[i]);
				center.push_back(glm::vec3((float)temp[i].x + (temp[i].y - temp[i].x) / 2.0f, (float)level + 1.9, (float)temp[i].z + (temp[i].w - temp[i].z) / 2.0f));
			}
			else if (temp[i].x != 11)
			{
				center[i] = glm::vec3((float)temp[i].x + (temp[i].y - temp[i].x) / 2.0f, (float)level + 1.9, (float)temp[i].z + (temp[i].w - temp[i].z) / 2.0f);
			}
		}
	}
	
	m_Rooms.push_back(new Room());
	m_Rooms.push_back(new Room());
	for (size_t i = 2; i < center.size(); i++)
	{
		m_Rooms.push_back(new Room(center[i]));
	}
}

void World::GenerateWater(Scene* pScene) noexcept
{
	for (int level = 0; level < m_NumLevels; level++)
	{
		m_ppLevels[level]->GenerateWater(pScene, level);
	}

	m_ppLevels[5]->GetFloodingIDs().push_back(glm::ivec2(10, 10));

	TileData* const * ppLevelData = m_ppLevels[5]->GetLevelData();
	//ppLevelData[10][10].WaterLevel = 0.05f;
	ppLevelData[10][10].WaterLevel = 1.0f;
	ppLevelData[10][10].AlreadyFlooded = true;
	//GameObject* pGameObject = scene->GetGameObject(ppLevelData[10][10].WaterBlockName);
	//pGameObject->SetScale(glm::vec3(0.0f, ppLevelData[10][10].WaterLevel, 0.0f));
	//pGameObject->UpdateTransform();
	//pGameObject->SetIsVisible(true);
}

void World::SetStairs(const glm::ivec3* stairs, uint32 nrOfStairs)
{
	m_NumStairs = nrOfStairs;
	m_pStairs = new glm::ivec3[m_NumStairs];

	for (int i = 0; i < m_NumStairs; i++) 
	{
		m_pStairs[i] = stairs[i];
	}
}

const glm::ivec3* World::GetStairs() const noexcept
{
	return m_pStairs;
}

uint32 World::GetNumStairs() const noexcept
{
	return m_NumStairs;
}

Room* World::GetRoom(uint32 room) const noexcept
{
	assert(room < m_Rooms.size());
	return m_Rooms[room];
}

void World::Update(Scene* pScene, float dt)
{
	//m_ppLevels[2]->UpdateFire(dt);

	for (uint32 levelIndex = 0; levelIndex < m_NumLevels; levelIndex++)
	{
		const uint32* const * ppLevel = m_ppLevels[levelIndex]->GetLevel();
		TileData* const * ppLevelData = m_ppLevels[levelIndex]->GetLevelData();
		std::vector<glm::ivec2>& floodingIDs = m_ppLevels[levelIndex]->GetFloodingIDs();
		glm::ivec2 levelSize = glm::ivec2(m_ppLevels[levelIndex]->GetSizeX(), m_ppLevels[levelIndex]->GetSizeZ());
		std::vector<glm::ivec2> newFloodingIDs;
		std::vector<glm::ivec2> toRemoveFloodingIDs;

		//TEMP
		if (levelIndex == 5/* && Input::IsKeyDown(KEY_I)*/)
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

		//We are on the upper grid level of a world level
		if (levelIndex % 2 == 1)
		{
			for (uint32 i = 0; i < floodingIDs.size(); i++)
			{
				glm::ivec2 currentTile = glm::ivec2(floodingIDs[i].x, floodingIDs[i].y);

				UpdateFloodingIdsBelow(m_ppLevels[levelIndex - 1], currentTile);
				UpdateWaterLevelBelow(m_ppLevels[levelIndex], m_ppLevels[levelIndex - 1], currentTile);

				//Remove Current Water Object (All the water flows down)
				ppLevelData[currentTile.x][currentTile.y].WaterLevel = 0.0f;
				ppLevelData[currentTile.x][currentTile.y].WaterLevelChange = 0.0f;
				ppLevelData[currentTile.x][currentTile.y].WaterLevelLastUpdated = 0.0f;
				ppLevelData[currentTile.x][currentTile.y].WaterLevelAge = 1.0f;
				ppLevelData[currentTile.x][currentTile.y].AlreadyFlooded = false;
				pScene->GetGameObject(ppLevelData[currentTile.x][currentTile.y].WaterBlockName)->SetIsVisible(false);
				toRemoveFloodingIDs.push_back(currentTile);
			}
		}
		//We are on the lower grid level of a world level
		else
		{

			for (uint32 i = 0; i < floodingIDs.size(); i++)
			{
				glm::ivec2 currentTile = glm::ivec2(floodingIDs[i].x, floodingIDs[i].y);
				bool hasStairsBelow = false;

				if (levelIndex > 0)
				{
					for (uint32 stairId = 0; stairId < m_NumStairs; stairId++)
					{
						if (levelIndex - m_pStairs[stairId].y == 2)
						{
							if (currentTile.x == m_pStairs[stairId].x && currentTile.y == m_pStairs[stairId].z)
							{
								UpdateFloodingIdsBelow(m_ppLevels[levelIndex - 1], currentTile);
								UpdateWaterLevelBelow(m_ppLevels[levelIndex], m_ppLevels[levelIndex - 1], currentTile);

								//Remove Current Water Object (All the water flows down)
								ppLevelData[currentTile.x][currentTile.y].WaterLevel = 0.0f;
								ppLevelData[currentTile.x][currentTile.y].WaterLevelChange = 0.0f;
								ppLevelData[currentTile.x][currentTile.y].WaterLevelLastUpdated = 0.0f;
								ppLevelData[currentTile.x][currentTile.y].WaterLevelAge = 1.0f;
								ppLevelData[currentTile.x][currentTile.y].AlreadyFlooded = false;
								pScene->GetGameObject(ppLevelData[currentTile.x][currentTile.y].WaterBlockName)->SetIsVisible(false);
								toRemoveFloodingIDs.push_back(currentTile);

								hasStairsBelow = true;
								break;
							}
						}
					}
				}

				if (!hasStairsBelow)
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

						float floodFactor = CalculateFloodFactor(waterLevelDifPosX, waterLevelDifNegX, waterLevelDifPosZ, waterLevelDifNegZ, dt);

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
		}

		for (uint32 i = 0; i < floodingIDs.size(); i++)
		{
			glm::ivec2 currentTile = glm::ivec2(floodingIDs[i].x, floodingIDs[i].y);

			ppLevelData[currentTile.x][currentTile.y].WaterLevel = glm::min<float>(ppLevelData[currentTile.x][currentTile.y].WaterLevel + ppLevelData[currentTile.x][currentTile.y].WaterLevelChange, 1.0f);
			ppLevelData[currentTile.x][currentTile.y].WaterLevelChange = 0.0f;

			//Evaporate(pScene, ppLevelData, toRemoveFloodingIDs, currentTile, dt);

			if (glm::abs(ppLevelData[currentTile.x][currentTile.y].WaterLevel - ppLevelData[currentTile.x][currentTile.y].WaterLevelLastUpdated) > WATER_UPDATE_INTERVAL)
			{
				ppLevelData[currentTile.x][currentTile.y].WaterLevelLastUpdated = (float)((uint32)(WATER_ROUNDING_FACTOR * ppLevelData[currentTile.x][currentTile.y].WaterLevel)) / WATER_ROUNDING_FACTOR;

				GameObject* pGameObject = pScene->GetGameObject(ppLevelData[currentTile.x][currentTile.y].WaterBlockName);
				float yOffset = levelIndex % 2 == 0 ? 0.05f : 0.0f;
				float yScale = glm::max(ppLevelData[currentTile.x][currentTile.y].WaterLevelLastUpdated, 0.05f);
				pGameObject->SetPosition(glm::vec3(currentTile.x, (float)levelIndex + yOffset + ppLevelData[currentTile.x][currentTile.y].WaterLevelLastUpdated / 2.0f, currentTile.y));
				pGameObject->SetScale(glm::vec3(1.0f, yScale, 1.0f));
				pGameObject->SetIsVisible(true);
				pGameObject->UpdateTransform();
			}
		}

		floodingIDs.insert(floodingIDs.end(), newFloodingIDs.begin(), newFloodingIDs.end());
		newFloodingIDs.clear();

		for (uint32 i = 0; i < toRemoveFloodingIDs.size(); i++)
		{
			floodingIDs.erase(std::remove(floodingIDs.begin(), floodingIDs.end(), toRemoveFloodingIDs[i]));
		}

		toRemoveFloodingIDs.clear();

		//std::cout << floodingIDs.size() << std::endl;
	}
}
