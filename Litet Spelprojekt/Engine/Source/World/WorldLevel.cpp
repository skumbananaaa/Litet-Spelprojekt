#include <EnginePch.h>
#include <World/WorldLevel.h>

WorldLevel::WorldLevel(uint32 levelHeight, const uint32* const levelIndexes, uint32 sizeX, uint32 sizeZ) noexcept
{
	m_SizeX = sizeX;
	m_SizeZ = sizeZ;
	m_ppLevelData = new TileData*[sizeX];
	m_ppLevel = new uint32*[sizeX];
	for (uint32 x = 0; x < m_SizeX; x++)
	{
		m_ppLevel[x] = new uint32[sizeZ];
		m_ppLevelData[x] = new TileData[sizeZ];
		for (uint32 z = 0; z < m_SizeZ; z++)
		{
			m_ppLevel[x][z] = levelIndexes[x * m_SizeZ + z];
			m_ppLevelData[x][z].HasStairs = false;
			m_ppLevelData[x][z].HasDoor = false;
			m_ppLevelData[x][z].BurnsAt = 100;
			m_ppLevelData[x][z].Temp = 30;
			m_ppLevelData[x][z].WaterLevel = 0.0f;
			m_ppLevelData[x][z].WaterLevelChange = 0.0f;
			m_ppLevelData[x][z].WaterLevelLastUpdated = 0.0f;
			m_ppLevelData[x][z].WaterLevelAge = 1.0f;
			m_ppLevelData[x][z].WaterFloodsFromeAbove = false;
			m_ppLevelData[x][z].AlreadyFlooded = false;
			m_ppLevelData[x][z].WaterBlockName = "WaterBlock [" + std::to_string(x) + ", " + std::to_string(levelHeight) + ", " + std::to_string(z) + "]";
		}
	}
}

WorldLevel::~WorldLevel()
{
	for (uint32 x = 0; x < m_SizeX; x++)
	{
		DeleteArrSafe(m_ppLevelData[x]);
		DeleteArrSafe(m_ppLevel[x]);
	}

	DeleteArrSafe(m_ppLevelData);
	DeleteArrSafe(m_ppLevel);
}

const uint32* const* const WorldLevel::GetLevel() const noexcept
{
	return m_ppLevel;
}

const TileData * const * const WorldLevel::GetLevelData() const noexcept
{
	return m_ppLevelData;
}

TileData * const * const WorldLevel::GetLevelData() noexcept
{
	return m_ppLevelData;
}

std::vector<glm::ivec2>& WorldLevel::GetBurningIDs() noexcept
{
	return m_BurningIDs;
}

std::vector<glm::ivec2>& WorldLevel::GetFloodingIDs() noexcept
{
	return m_FloodingIDs;
}

uint32 WorldLevel::GetSizeX() const noexcept
{
	return m_SizeX;
}

uint32 WorldLevel::GetSizeZ() const noexcept
{
	return m_SizeZ;
}

const glm::vec4& WorldLevel::GetWall(uint32 index) const noexcept
{
	assert(index < m_Walls.size());
	return m_Walls[index];
}

uint32 WorldLevel::GetNrOfWalls() const noexcept
{
	return m_Walls.size();
}

const std::vector<glm::uvec4>& WorldLevel::GetRooms() const noexcept
{
	return m_RoomBounds;
}

void WorldLevel::GenerateRooms()
{
	bool wallH = false, wallV = false;
	glm::vec2 startWallH(0, 0), endWallH(0, 0);
	glm::vec2 startWallV(0, 0), endWallV(0, 0);

	uint32 maxRoomNum = 0;

	for (uint32 i = 0; i < m_SizeX - 1; i++) {
		for (uint32 j = 0; j < m_SizeZ; j++) {
			maxRoomNum = glm::max(maxRoomNum, m_ppLevel[i][j]);

			wallH = (m_ppLevel[i][j] != m_ppLevel[i + 1][j]);
			if ((!wallH || (m_ppLevelData[i][j].HasDoor && m_ppLevelData[i + 1][j].HasDoor) || m_ppLevel[i][j] != m_ppLevel[i][j - 1] || m_ppLevel[i + 1][j] != m_ppLevel[i + 1][j - 1]) && startWallH != glm::vec2(0, 0))
			{
				endWallH = glm::vec2(i + 0.5, j - 0.5);
				m_Walls.push_back(glm::vec4((startWallH + endWallH) / 2.0f, endWallH - startWallH));
				startWallH = glm::vec2(0, 0);
			}
			if (wallH && startWallH == glm::vec2(0, 0) && (!m_ppLevelData[i][j].HasDoor || !m_ppLevelData[i + 1][j].HasDoor))
			{
				startWallH = glm::vec2(i + 0.5, j - 0.5);
			}
		}
	}

	for (uint32 i = m_RoomBounds.size(); i <= maxRoomNum; i++)
	{
		m_RoomBounds.push_back(glm::uvec4(11, 0, 41, 0));
	}

	for (uint32 i = 0; i < m_SizeZ - 1; i++) {
		for (uint32 j = 0; j < m_SizeX; j++) {

			m_RoomBounds[m_ppLevel[j][i]].x = glm::min(m_RoomBounds[m_ppLevel[j][i]].x, j);
			m_RoomBounds[m_ppLevel[j][i]].y = glm::max(m_RoomBounds[m_ppLevel[j][i]].y, j);
			m_RoomBounds[m_ppLevel[j][i]].z = glm::min(m_RoomBounds[m_ppLevel[j][i]].z, i);
			m_RoomBounds[m_ppLevel[j][i]].w = glm::max(m_RoomBounds[m_ppLevel[j][i]].w, i);

			wallV = (m_ppLevel[j][i] != m_ppLevel[j][i + 1]);
			if ((!wallV || (m_ppLevelData[j][i].HasDoor && m_ppLevelData[j][i + 1].HasDoor) || m_ppLevel[j][i] != m_ppLevel[j - 1][i] || (m_ppLevel[j][i + 1] != m_ppLevel[j - 1][i + 1])) && startWallV != glm::vec2(0, 0))
			{
				endWallV = glm::vec2(j - 0.5, i + 0.5);
				m_Walls.push_back(glm::vec4((startWallV + endWallV) / 2.0f, endWallV - startWallV));
				startWallV = glm::vec2(0, 0);
			}
			if (wallV && startWallV == glm::vec2(0, 0) && (!m_ppLevelData[j][i].HasDoor || !m_ppLevelData[j][i + 1].HasDoor))
			{
				startWallV = glm::vec2(j - 0.5, i + 0.5);
			}
		}
	}
}
void WorldLevel::GenerateWater(Scene* pScene, uint32 levelHeight)
{
	WaterObject* pGameObject = nullptr;

	for (uint32 x = 0; x < m_SizeX; x++)
	{
		for (uint32 z = 0; z < m_SizeZ; z++)
		{
			pGameObject = new WaterObject();
			pGameObject->SetIsReflectable(true);
			pGameObject->SetIsVisible(false);
			pGameObject->SetMesh(MESH::CUBE);
			pGameObject->SetMaterial(MATERIAL::OCEAN_BLUE);
			pGameObject->SetScale(glm::vec3(1.0f));
			pGameObject->SetPosition(glm::vec3(x, levelHeight, z));
			pGameObject->UpdateTransform();
			pGameObject->SetName(m_ppLevelData[x][z].WaterBlockName);
			pScene->AddGameObject(pGameObject);
		}
	}
}

//en optimering hade varit att enbart kolla angränsade tiles till de tiles som brinner istället för att kolla 
//alla tiles i hela vår grid. 
void WorldLevel::UpdateFire(float dt)
{
	// Ej optimerad.
	/*for (uint32 x = 0; x < m_SizeX; x++)
	{
		for (uint32 z = 0; z < m_SizeZ; z++)
		{
			if (x + 1 < m_SizeX)
			{
				if (m_ppLevel[x + 1][z] == m_ppLevel[x][z] || m_ppLevel[x][z] == 0 || m_ppLevel[x + 1][z] == 0)
				{
					m_ppLevelData[x][z].Temp += std::fmaxf((m_ppLevelData[x + 1][z].Temp - m_ppLevelData[x][z].Temp)*dt, 0.0f);
				}
			}

			if (x > 0)
			{
				if (m_ppLevel[x - 1][z] == m_ppLevel[x][z] || m_ppLevel[x][z] == 0 || m_ppLevel[x - 1][z] == 0)
				{
					m_ppLevelData[x][z].Temp += std::fmaxf((m_ppLevelData[x - 1][z].Temp - m_ppLevelData[x][z].Temp)*dt, 0.0f);
				}
			}

			if (z + 1 < m_SizeZ)
			{
				if (m_ppLevel[x][z + 1] == m_ppLevel[x][z] || m_ppLevel[x][z] == 0 || m_ppLevel[x][z + 1] == 0)
				{
					m_ppLevelData[x][z].Temp += std::fmaxf((m_ppLevelData[x][z + 1].Temp - m_ppLevelData[x][z].Temp)*dt, 0.0f);
				}
			}

			if (z > 0)
			{
				if (m_ppLevel[x][z - 1] == m_ppLevel[x][z] || m_ppLevel[x][z] == 0 || m_ppLevel[1][z - 1] == 0)
				{
					m_ppLevelData[x][z].Temp += std::fmaxf((m_ppLevelData[x][z - 1].Temp- m_ppLevelData[x][z].Temp)*dt, 0.0f);
				}
			}
		}
	}*/

	// optimerad version.
	for (uint32 i = 0; i < m_BurningIDs.size(); i++)
	{
		uint32 x = m_BurningIDs[i].x;
		uint32 z = m_BurningIDs[i].y;
		bool alreadyBurning = false;
		if (x + 1 < m_SizeX)
		{
			if (m_ppLevel[x + 1][z] == m_ppLevel[x][z] || m_ppLevel[x][z] == 0 || m_ppLevel[x + 1][z] == 0)
			{
				if (m_ppLevelData[x + 1][z].Temp >= m_ppLevelData[x + 1][z].BurnsAt)
				{
					alreadyBurning = true;
				}

				m_ppLevelData[x + 1][z].Temp += std::fmaxf((m_ppLevelData[x][z].Temp - m_ppLevelData[x + 1][z].Temp) * dt, 0.0f);

				if (m_ppLevelData[x + 1][z].Temp >= m_ppLevelData[x + 1][z].BurnsAt && !alreadyBurning)
				{
					m_BurningIDs.push_back(glm::ivec2(x + 1, z));
				}
			}
		}

		if (x > 0)
		{
			if (m_ppLevel[x - 1][z] == m_ppLevel[x][z] || m_ppLevel[x][z] == 0 || m_ppLevel[x - 1][z] == 0)
			{
				if (m_ppLevelData[x - 1][z].Temp >= m_ppLevelData[x - 1][z].BurnsAt)
				{
					alreadyBurning = true;
				}

				m_ppLevelData[x - 1][z].Temp += std::fmaxf((m_ppLevelData[x][z].Temp - m_ppLevelData[x - 1][z].Temp) * dt, 0.0f);

				if (m_ppLevelData[x - 1][z].Temp >= m_ppLevelData[x - 1][z].BurnsAt && !alreadyBurning)
				{
					m_BurningIDs.push_back(glm::ivec2(x - 1, z));
				}
			}
		}

		if (z + 1 < m_SizeZ)
		{
			if (m_ppLevel[x][z + 1] == m_ppLevel[x][z] || m_ppLevel[x][z] == 0 || m_ppLevel[x][z + 1] == 0)
			{
				if (m_ppLevelData[x][z + 1].Temp >= m_ppLevelData[x][z + 1].BurnsAt)
				{
					alreadyBurning = true;
				}

				m_ppLevelData[x][z + 1].Temp += std::fmaxf((m_ppLevelData[x][z].Temp - m_ppLevelData[x][z + 1].Temp) * dt, 0.0f);

				if (m_ppLevelData[x][z + 1].Temp >= m_ppLevelData[x][z + 1].BurnsAt && !alreadyBurning)
				{
					m_BurningIDs.push_back(glm::ivec2(x, z - 1));
				}
			}
		}

		if (z > 0)
		{
			if (m_ppLevel[x][z - 1] == m_ppLevel[x][z] || m_ppLevel[x][z] == 0 || m_ppLevel[1][z - 1] == 0)
			{
				if (m_ppLevelData[x][z - 1].Temp >= m_ppLevelData[x][z - 1].BurnsAt)
				{
					alreadyBurning = true;
				}

				m_ppLevelData[x][z - 1].Temp += std::fmaxf((m_ppLevelData[x][z].Temp - m_ppLevelData[x][z - 1].Temp) * dt, 0.0f);

				if (m_ppLevelData[x][z - 1].Temp >= m_ppLevelData[x][z - 1].BurnsAt && !alreadyBurning)
				{
					m_BurningIDs.push_back(glm::ivec2(x, z - 1));
				}
			}
		}
	}
}

// 1 liter vatten i hundra grader ger 1800 volymsexpansion. Ta med detta i kalkylationer?
void WorldLevel::UpdateSmoke(float dt, const TileData* const* fireLevel, WorldLevel* aboveLevel)
{
	for (uint32 x = 0; x < m_SizeX; x++)
	{
		for (uint32 z = 0; z < m_SizeZ; z++)
		{
			if (fireLevel[x][z].Temp >= fireLevel[x][z].BurnsAt)
			{
				m_ppLevelData[x][z].SmokeAmount += (fireLevel[x][z].Temp - fireLevel[x][z].BurnsAt)*dt;
			}

			if (m_ppLevelData[x][z].SmokeAmount > m_ppLevelData[x][z].SmokeLimit)
			{
				float spread = (m_ppLevelData[x][z].SmokeAmount - m_ppLevelData[x][z].SmokeLimit) / 4;

				// assuming that the value of 1 is walls...
				if (aboveLevel->m_ppLevel[x][z] != 1)
				{
					aboveLevel->m_ppLevelData[x][z].Temp += spread * dt;
				}

				// Smoke leaves the air right?
				m_ppLevelData[x][z].SmokeAmount -= spread;

				if (x + 1 < m_SizeX)
				{
					if (m_ppLevel[x + 1][z] == m_ppLevel[x][z] || m_ppLevel[x][z] == 0 || m_ppLevel[x + 1][z] == 0)
					{
						m_ppLevelData[x + 1][z].SmokeAmount += spread;
					}
				}

				if (x > 0)
				{
					if (m_ppLevel[x - 1][z] == m_ppLevel[x][z] || m_ppLevel[x][z] == 0 || m_ppLevel[x - 1][z] == 0)
					{
						m_ppLevelData[x - 1][z].SmokeAmount += spread;
					}
				}

				if (z + 1 < m_SizeZ)
				{
					if (m_ppLevel[x][z + 1] == m_ppLevel[x][z] || m_ppLevel[x][z] == 0 || m_ppLevel[x][z + 1] == 0)
					{
						m_ppLevelData[x][z + 1].SmokeAmount += spread;
					}
				}

				if (z > 0)
				{
					if (m_ppLevel[x][z - 1] == m_ppLevel[x][z] || m_ppLevel[x][z] == 0 || m_ppLevel[1][z - 1] == 0)
					{
						m_ppLevelData[x][z - 1].SmokeAmount += spread;
					}
				}
			}
		}
	}
}

