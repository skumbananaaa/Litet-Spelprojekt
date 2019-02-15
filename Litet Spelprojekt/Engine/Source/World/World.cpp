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
	std::vector<glm::uvec4> roomBounds;
	std::vector<glm::uvec4> temp;

	std::vector<glm::vec3> center;

	for (int level = 0; level < m_NumLevels; level += 2)
	{
		m_ppLevels[level]->GenerateRooms();
		temp = m_ppLevels[level]->GetRooms();
		for (size_t i = 0; i < temp.size(); i++)
		{
			if (i >= roomBounds.size())
			{
				roomBounds.push_back(temp[i]);
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

	m_ppLevels[4]->GetFloodingIDs().push_back(glm::ivec2(10, 10));

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

void World::Update(Scene* scene, float dt)
{
	//m_ppLevels[2]->UpdateFire(dt);

	for (uint32 levelIndex = 0; levelIndex < m_NumLevels; levelIndex++)
	{
		const uint32* const * ppLevel = m_ppLevels[levelIndex]->GetLevel();
		TileData* const * ppLevelData = m_ppLevels[levelIndex]->GetLevelData();
		std::vector<glm::ivec2>& burningIDs = m_ppLevels[levelIndex]->GetBurningIDs();
		std::vector<glm::ivec2>& floodingIDs = m_ppLevels[levelIndex]->GetFloodingIDs();
		glm::ivec2 levelSize = glm::ivec2(m_ppLevels[levelIndex]->GetSizeX(), m_ppLevels[levelIndex]->GetSizeZ());

		//TEMP
		if (levelIndex == 4)
		{
			ppLevelData[10][10].WaterLevel = 1.0f;
			ppLevelData[10][10].AlreadyFlooded = true;
			GameObject* pGameObject = scene->GetGameObject(ppLevelData[10][10].WaterBlockName);
			pGameObject->SetScale(glm::vec3(0.0f, ppLevelData[10][10].WaterLevel, 0.0f));
			pGameObject->UpdateTransform();
			pGameObject->SetIsVisible(true);
		}

		std::vector<glm::ivec2> newFloodingIDs;
		for (uint32 i = 0; i < floodingIDs.size(); i++)
		{
			glm::ivec2 currentTile = glm::ivec2(floodingIDs[i].x, floodingIDs[i].y);

			if (ppLevelData[currentTile.x][currentTile.y].WaterLevel > 0.1f)
			{
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

				float floodFactor = CalculateFloodFactor(waterLevelDifPosX, waterLevelDifNegX, waterLevelDifPosZ, waterLevelDifNegZ) * dt;
				//std::cout << "floodFactor: " << std::to_string(floodFactor) << std::endl;

				//std::cout << "Water level: " << std::to_string(ppLevelData[tilePosX.x][tilePosX.y].WaterLevel) << " at: " << std::to_string(tilePosX.x) << ", " << std::to_string(levelIndex) << ", " << std::to_string(tilePosX.y)  << std::endl;

				//POS X
				if (canSpreadToPosX == 1)
				{
					if (!ppLevelData[tilePosX.x][tilePosX.y].AlreadyFlooded)
					{
						newFloodingIDs.push_back(tilePosX);
						ppLevelData[tilePosX.x][tilePosX.y].AlreadyFlooded = true;
					}
				}

				if (waterLevelDifPosX > 0.0f)
				{
					ppLevelData[currentTile.x][currentTile.y].WaterLevel = glm::max(0.0f, ppLevelData[currentTile.x][currentTile.y].WaterLevel - floodFactor);
					ppLevelData[currentTile.x][currentTile.y].WaterLevelChanged = true;

					ppLevelData[tilePosX.x][tilePosX.y].WaterLevel = glm::min(1.0f, ppLevelData[tilePosX.x][tilePosX.y].WaterLevel + floodFactor);
					ppLevelData[tilePosX.x][tilePosX.y].WaterLevelChanged = true;

					if (ppLevelData[tilePosX.x][tilePosX.y].WaterLevel > 0.0f)
					{
						scene->GetGameObject(ppLevelData[tilePosX.x][tilePosX.y].WaterBlockName)->SetIsVisible(true);
					}
				}

				//NEG X
				if (canSpreadToNegX == 1)
				{
					if (!ppLevelData[tileNegX.x][tileNegX.y].AlreadyFlooded)
					{
						newFloodingIDs.push_back(tileNegX);
						ppLevelData[tileNegX.x][tileNegX.y].AlreadyFlooded = true;
					}
				}

				if (waterLevelDifNegX > 0.0f)
				{
					ppLevelData[currentTile.x][currentTile.y].WaterLevel = glm::max(0.0f, ppLevelData[currentTile.x][currentTile.y].WaterLevel - floodFactor);
					ppLevelData[currentTile.x][currentTile.y].WaterLevelChanged = true;

					ppLevelData[tileNegX.x][tileNegX.y].WaterLevel = glm::min(1.0f, ppLevelData[tileNegX.x][tileNegX.y].WaterLevel + floodFactor);
					ppLevelData[tileNegX.x][tileNegX.y].WaterLevelChanged = true;

					if (ppLevelData[tileNegX.x][tileNegX.y].WaterLevel > 0.0f)
					{
						scene->GetGameObject(ppLevelData[tileNegX.x][tileNegX.y].WaterBlockName)->SetIsVisible(true);
					}
				}

				//POS Z
				if (canSpreadToPosZ == 1)
				{
					if (!ppLevelData[tilePosZ.x][tilePosZ.y].AlreadyFlooded)
					{
						newFloodingIDs.push_back(tilePosZ);
						ppLevelData[tilePosZ.x][tilePosZ.y].AlreadyFlooded = true;
					}
				}

				if (waterLevelDifPosZ > 0.0f)
				{
					ppLevelData[currentTile.x][currentTile.y].WaterLevel = glm::max(0.0f, ppLevelData[currentTile.x][currentTile.y].WaterLevel - floodFactor);
					ppLevelData[currentTile.x][currentTile.y].WaterLevelChanged = true;

					ppLevelData[tilePosZ.x][tilePosZ.y].WaterLevel = glm::min(1.0f, ppLevelData[tilePosZ.x][tilePosZ.y].WaterLevel + floodFactor);
					ppLevelData[tilePosZ.x][tilePosZ.y].WaterLevelChanged = true;

					if (ppLevelData[tilePosZ.x][tilePosZ.y].WaterLevel > 0.0f)
					{
						scene->GetGameObject(ppLevelData[tilePosZ.x][tilePosZ.y].WaterBlockName)->SetIsVisible(true);
					}
				}

				//NEG Z
				if (waterLevelDifNegZ > 0.0f)
				{
					ppLevelData[currentTile.x][currentTile.y].WaterLevel = glm::max(0.0f, ppLevelData[currentTile.x][currentTile.y].WaterLevel - floodFactor);
					ppLevelData[currentTile.x][currentTile.y].WaterLevelChanged = true;

					ppLevelData[tileNegZ.x][tileNegZ.y].WaterLevel = glm::min(1.0f, ppLevelData[tileNegZ.x][tileNegZ.y].WaterLevel + floodFactor);
					ppLevelData[tileNegZ.x][tileNegZ.y].WaterLevelChanged = true;

					if (ppLevelData[tileNegZ.x][tileNegZ.y].WaterLevel > 0.0f)
					{
						scene->GetGameObject(ppLevelData[tileNegZ.x][tileNegZ.y].WaterBlockName)->SetIsVisible(true);
					}
				}

				if (canSpreadToNegZ == 1)
				{
					if (!ppLevelData[tileNegZ.x][tileNegZ.y].AlreadyFlooded)
					{
						newFloodingIDs.push_back(tileNegZ);
						ppLevelData[tileNegZ.x][tileNegZ.y].AlreadyFlooded = true;
					}
				}

				GameObject* pGameObject = scene->GetGameObject(ppLevelData[currentTile.x][currentTile.y].WaterBlockName);
				pGameObject->SetIsVisible(ppLevelData[currentTile.x][currentTile.y].WaterLevel > 0.0f);
			}
		}

		floodingIDs.insert(floodingIDs.end(), newFloodingIDs.begin(), newFloodingIDs.end());
		newFloodingIDs.clear();

		for (uint32 i = 0; i < floodingIDs.size(); i++)
		{
			glm::ivec2 currentTile = glm::ivec2(floodingIDs[i].x, floodingIDs[i].y);

			if (ppLevelData[currentTile.x][currentTile.y].WaterLevelChanged)
			{
				ppLevelData[currentTile.x][currentTile.y].WaterLevelChanged = false;
				GameObject* pGameObject = scene->GetGameObject(ppLevelData[currentTile.x][currentTile.y].WaterBlockName);
				pGameObject->SetPosition(glm::vec3(currentTile.x, levelIndex + ppLevelData[currentTile.x][currentTile.y].WaterLevel / 2.0f, currentTile.y));
				pGameObject->SetScale(glm::vec3(1.0f, ppLevelData[currentTile.x][currentTile.y].WaterLevel, 1.0f));
				pGameObject->UpdateTransform();
			}
		}

		//std::cout << floodingIDs.size() << std::endl;
	}
}

uint32 World::CanSpreadTo(const uint32 * const * ppLevel, const glm::ivec2& levelSize, const glm::ivec2& tileFrom, const glm::ivec2& tileTo) const noexcept
{
	if (tileTo.x < 0 || tileTo.x >= levelSize.x || tileTo.y < 0 || tileTo.y >= levelSize.y)
	{
		return 0;
	}

	if (ppLevel[tileFrom.x][tileFrom.y] == ppLevel[tileTo.x][tileTo.y] || ppLevel[tileFrom.x][tileFrom.y] == 0 || ppLevel[tileTo.x][tileTo.y] == 0)
	{
		return 1;
	}

	return 0;
}

float World::CanFloodTo(const uint32 * const * ppLevel, const TileData * const * ppLevelData,  const glm::ivec2& levelSize, const glm::ivec2& tileFrom, const glm::ivec2& tileTo, uint32 canSpreadTo) const noexcept
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

float World::CalculateFloodFactor(float waterLevelDifPosX, float waterLevelDifNegX, float waterLevelDifPosZ, float waterLevelDifNegZ) const noexcept
{
	float denominator = 0.0f;
	denominator += waterLevelDifPosX > 0.0f ? 1.0f : 0.0f;
	denominator += waterLevelDifNegX > 0.0f ? 1.0f : 0.0f;
	denominator += waterLevelDifPosZ > 0.0f ? 1.0f : 0.0f;
	denominator += waterLevelDifNegZ > 0.0f ? 1.0f : 0.0f;

	float nominator = 0.0f;
	nominator += waterLevelDifPosX;
	nominator += waterLevelDifNegX;
	nominator += waterLevelDifPosZ;
	nominator += waterLevelDifNegZ;

	if (!(denominator > 0.0f))
	{
		return 0.0f;
	}

	return nominator / denominator;
}
