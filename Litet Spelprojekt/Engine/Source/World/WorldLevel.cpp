#include <EnginePch.h>
#include <World/WorldLevel.h>

WorldLevel::WorldLevel(WorldLevel&& other)
{
	m_SizeX = other.m_SizeX;
	m_SizeZ = other.m_SizeZ;
	m_TilesBetweenBulkheads = other.m_TilesBetweenBulkheads;

	m_ppLevel = other.m_ppLevel;
	m_ppLevelData = other.m_ppLevelData;

	m_Walls.swap(other.m_Walls);
	m_Bulkheads.swap(other.m_Bulkheads);
}

WorldLevel::WorldLevel(const WorldLevel& other)
{
	m_Walls = other.m_Walls;
	m_Bulkheads = other.m_Bulkheads;

	m_SizeX = other.m_SizeX;
	m_SizeZ = other.m_SizeZ;
	m_TilesBetweenBulkheads = other.m_TilesBetweenBulkheads;

	m_ppLevelData = new TileData*[m_SizeX];
	m_ppLevel = new uint32*[m_SizeX];
	for (uint32 x = 0; x < m_SizeX; x++)
	{
		m_ppLevel[x] = new uint32[m_SizeZ];
		m_ppLevelData[x] = new TileData[m_SizeZ];
		for (uint32 z = 0; z < m_SizeZ; z++)
		{
			m_ppLevel[x][z] = other.m_ppLevel[x][z];
			m_ppLevelData[x][z].HasStairs = other.m_ppLevelData[x][z].HasStairs;
			m_ppLevelData[x][z].BurnsAt = other.m_ppLevelData[x][z].BurnsAt;
			m_ppLevelData[x][z].Temp = other.m_ppLevelData[x][z].Temp;
			m_ppLevelData[x][z].SmokeAmount = other.m_ppLevelData[x][z].SmokeAmount;
			m_ppLevelData[x][z].SmokeLimit = other.m_ppLevelData[x][z].SmokeLimit;
			m_ppLevelData[x][z].WaterLevel = other.m_ppLevelData[x][z].WaterLevel;
			m_ppLevelData[x][z].WaterLevelChange = other.m_ppLevelData[x][z].WaterLevelChange;
			m_ppLevelData[x][z].WaterLevelLastUpdated = other.m_ppLevelData[x][z].WaterLevelLastUpdated;
			m_ppLevelData[x][z].WaterLevelAge = other.m_ppLevelData[x][z].WaterLevelAge;
			m_ppLevelData[x][z].AlreadyFlooded = other.m_ppLevelData[x][z].AlreadyFlooded;
			m_ppLevelData[x][z].Burning = other.m_ppLevelData[x][z].Burning;
			m_ppLevelData[x][z].WaterInlet = other.m_ppLevelData[x][z].WaterInlet;
			m_ppLevelData[x][z].MarkedForExtinguish = other.m_ppLevelData[x][z].MarkedForExtinguish;
		}
	}
}

WorldLevel::WorldLevel(uint32 levelHeight, const uint32* const levelIndexes, uint32 sizeX, uint32 sizeZ) noexcept 
	: m_TilesBetweenBulkheads(0),
	m_ppLevel(nullptr),
	m_ppLevelData(nullptr)
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
			m_ppLevelData[x][z].BurnsAt = 100;
			m_ppLevelData[x][z].Temp = 30;
			m_ppLevelData[x][z].SmokeAmount = 0;
			m_ppLevelData[x][z].SmokeLimit = 100;
			m_ppLevelData[x][z].WaterLevel = 0.0f;
			m_ppLevelData[x][z].WaterLevelChange = 0.0f;
			m_ppLevelData[x][z].WaterLevelLastUpdated = 0.0f;
			m_ppLevelData[x][z].WaterLevelAge = 1.0f;
			m_ppLevelData[x][z].AlreadyFlooded = false;
			m_ppLevelData[x][z].Burning = false;
			m_ppLevelData[x][z].WaterInlet = false;
			m_ppLevelData[x][z].MarkedForExtinguish = false;
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

const TileData* const * const WorldLevel::GetLevelData() const noexcept
{
	return m_ppLevelData;
}

TileData * const * const WorldLevel::GetLevelData() noexcept
{
	return m_ppLevelData;
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
	return static_cast<uint32>(m_Walls.size());
}

const glm::vec4& WorldLevel::GetBulkhead(uint32 index) const noexcept
{
	assert(index < m_Bulkheads.size());
	return m_Bulkheads[index];
}

uint32 WorldLevel::GetNrOfBulkheads() const noexcept
{
	return m_Bulkheads.size();
}

uint32 WorldLevel::GetTilesBetweenBulkheads() const noexcept
{
	return m_TilesBetweenBulkheads;
}

std::vector<glm::uvec4> WorldLevel::GenerateRooms(uint32 tilesBetweenBulkheads)
{
	bool wallH = false;
	bool wallV = false;
	glm::vec2 startWallH(0, 0);
	glm::vec2 endWallH(0, 0);

	glm::vec2 startWallV(0, 0);
	glm::vec2 endWallV(0, 0);

	std::vector<glm::uvec4> roomBounds;

	//Generate walls along the Z Axis
	for (uint32 i = 0; i < m_SizeX - 1; i++) 
	{
		for (uint32 j = 0; j < m_SizeZ; j++) 
		{
			wallH = (m_ppLevel[i][j] != m_ppLevel[i + 1][j]);
			if ((!wallH || (m_ppLevelData[i][j].HasDoor() && m_ppLevelData[i + 1][j].HasDoor()) || m_ppLevel[i][j] != m_ppLevel[i][j - 1] || m_ppLevel[i + 1][j] != m_ppLevel[i + 1][j - 1]) && startWallH != glm::vec2(0, 0))
			{
				endWallH = glm::vec2(i + 0.5f, j - 0.5f);
				m_Walls.push_back(glm::vec4((startWallH + endWallH) / 2.0f, endWallH - startWallH));
				startWallH = glm::vec2(0, 0);
			}

			if (wallH && startWallH == glm::vec2(0, 0) && (!m_ppLevelData[i][j].HasDoor() || !m_ppLevelData[i + 1][j].HasDoor()))
			{
				startWallH = glm::vec2(i + 0.5f, j - 0.5f);
			}
		}
	}

	for (uint32 i = 0; i <= MAX_NUM_ROOMS; i++)
	{
		roomBounds.push_back(glm::uvec4(11, 0, 41, 0));
	}

	//Generate walls along the X Axis
	for (uint32 i = 0; i < m_SizeZ - 1; i++) 
	{
		for (uint32 j = 0; j < m_SizeX; j++) 
		{
			roomBounds[m_ppLevel[j][i]].x = glm::min(roomBounds[m_ppLevel[j][i]].x, j);
			roomBounds[m_ppLevel[j][i]].y = glm::max(roomBounds[m_ppLevel[j][i]].y, j);
			roomBounds[m_ppLevel[j][i]].z = glm::min(roomBounds[m_ppLevel[j][i]].z, i);
			roomBounds[m_ppLevel[j][i]].w = glm::max(roomBounds[m_ppLevel[j][i]].w, i);

			wallV = (m_ppLevel[j][i] != m_ppLevel[j][i + 1]);
			if ((!wallV || (m_ppLevelData[j][i].HasDoor() && m_ppLevelData[j][i + 1].HasDoor()) || m_ppLevel[j][i] != m_ppLevel[j - 1][i] || (m_ppLevel[j][i + 1] != m_ppLevel[j - 1][i + 1])) && startWallV != glm::vec2(0, 0))
			{
				float endXOffset = -0.6f;
				
				if (j < m_SizeX - 1)
				{
					if (m_ppLevelData[j][i].HasDoor() && m_ppLevelData[j][i + 1].HasDoor())
					{
						endXOffset = -0.5f;
					} 
				}

				endWallV = glm::vec2(j + endXOffset, i + 0.5f);

				if (((uint32)(startWallV.y - 0.5f)) % tilesBetweenBulkheads != 0)
				{
					m_Walls.push_back(glm::vec4((startWallV + endWallV) / 2.0f, endWallV - startWallV));
				}

				startWallV = glm::vec2(0, 0);
			}
			if (wallV && startWallV == glm::vec2(0, 0) && (!m_ppLevelData[j][i].HasDoor() || !m_ppLevelData[j][i + 1].HasDoor()))
			{
				float startXOffset = -0.4f;

				if (j > 0)
				{
					if (m_ppLevelData[j - 1][i].HasDoor() && m_ppLevelData[j - 1][i + 1].HasDoor())
					{
						startXOffset = -0.5f;
					}
				}

				startWallV = glm::vec2(j + startXOffset, i + 0.5f);
			}
		}
	}

	//Generate Bulkheads
	m_TilesBetweenBulkheads = tilesBetweenBulkheads;
	assert((m_SizeZ - 2) % tilesBetweenBulkheads == 0);
	uint32 nrOfBulkheads = 1 + (m_SizeZ - 2) / tilesBetweenBulkheads;
	uint32 currentBulkheadX = 0;

	bool createBulkhead = true;
	glm::vec2 startBulkhead(0, 0);
	glm::vec2 endBulkhead(0, 0);

	for (uint32 i = 0; i < nrOfBulkheads; i++)
	{
		for (uint32 j = 1; j < m_SizeX; j++)
		{
			if (!createBulkhead && (j == m_SizeX - 1 || (m_ppLevelData[j][currentBulkheadX].HasDoor() && m_ppLevelData[j][currentBulkheadX + 1].HasDoor())))
			{
				float endXOffset = -0.6f;
					
				if (j < m_SizeX - 1)
				{
					if (m_ppLevelData[j][currentBulkheadX].HasDoor() && m_ppLevelData[j][currentBulkheadX + 1].HasDoor())
					{
						endXOffset = -0.5f;
					}
				}

				endBulkhead = glm::vec2(j + endXOffset, currentBulkheadX + 0.5f);
				m_Bulkheads.push_back(glm::vec4((startBulkhead + endBulkhead) / 2.0f, endBulkhead - startBulkhead));

				createBulkhead = true;
			}

			if (createBulkhead && (!m_ppLevelData[j][currentBulkheadX].HasDoor() || !m_ppLevelData[j][currentBulkheadX + 1].HasDoor()))
			{

				float startXOffset = -0.4f;

				if (j > 0)
				{
					if (m_ppLevelData[j - 1][currentBulkheadX].HasDoor() && m_ppLevelData[j - 1][currentBulkheadX + 1].HasDoor())
					{
						startXOffset = -0.5f;
					}
				}

				startBulkhead = glm::vec2(j + startXOffset, currentBulkheadX + 0.5f);
				createBulkhead = false;
			}
		}

		currentBulkheadX += tilesBetweenBulkheads;
		createBulkhead = true;
	}

	return roomBounds;
}

void WorldLevel::GenerateScenarioObjects(Scene* pScene, uint32 levelHeight)
{
	GameObject* pGameObject = nullptr;
	MeshEmitter* pEmitter = nullptr;
	bool evenLevel = levelHeight % 2 == 0;
	bool oddLevel = levelHeight % 2 == 1;

	for (uint32 x = 0; x < m_SizeX; x++)
	{
		for (uint32 z = 0; z < m_SizeZ; z++)
		{
			if (evenLevel)
			{
				//Water
				pGameObject = new GameObject();
				pGameObject->SetMesh(MESH::CUBE);
				pGameObject->SetMaterial(MATERIAL::WATER_INDOOR);
				pGameObject->SetScale(glm::vec3(1.0f));
				pGameObject->SetPosition(glm::vec3(x, levelHeight, z));
				pGameObject->UpdateTransform();
				pGameObject->SetIsVisible(false);
				pScene->AddGameObject(pGameObject);
				m_ppLevelData[x][z].GameObjects[GAMEOBJECT_CONST_INDEX_WATER] = pGameObject;
			}

			if (oddLevel)
			{
				//Smoke
				pEmitter = new MeshEmitter();
				pEmitter->SetMesh(MESH::MESH_PARTICLE);
				pEmitter->SetTimeToLive(2.4f);
				pEmitter->SetConeAngle(glm::radians<float>(90.0f));
				pEmitter->SetSpeed(0.1f, 0.4f);
				pEmitter->SetScale(glm::vec2(0.1f), glm::vec2(0.3f));
				pEmitter->SetBeginColor(glm::vec4(0.2f, 0.2f, 0.2f, 0.3f));
				pEmitter->SetEndColor(glm::vec4(0.05f, 0.05f, 0.05f, 0.3f));
				pEmitter->SetPosition(glm::vec3(x, levelHeight + 0.3f, z));
				pEmitter->SetParticlesPerSeconds(4);
				pEmitter->UpdateTransform();
				pEmitter->SetIsVisible(false);
				pScene->AddGameObject(pEmitter);
				m_ppLevelData[x][z].GameObjects[GAMEOBJECT_CONST_INDEX_SMOKE] = pEmitter;
			}

			if (evenLevel)
			{
				//Fire
				pEmitter = new MeshEmitter();
				pEmitter->SetMesh(MESH::MESH_PARTICLE);
				pEmitter->SetTimeToLive(0.7f);
				pEmitter->SetConeAngle(glm::radians<float>(30.0f));
				pEmitter->SetSpeed(0.7f, 2.0f);
				pEmitter->SetScale(glm::vec2(0.1f), glm::vec2(0.2f));
				pEmitter->SetBeginColor(glm::vec4(1.0f, 1.0f, 0.3f, 1.0f));
				pEmitter->AddColorNode(glm::vec4(1.0f, 0.92f, 0.03f, 1.0f), 0.3f);
				pEmitter->SetEndColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
				pEmitter->SetPosition(glm::vec3(x, levelHeight, z));
				pEmitter->SetParticlesPerSeconds(16);
				pEmitter->UpdateTransform();
				pEmitter->SetIsVisible(false);
				pScene->AddGameObject(pEmitter);
				m_ppLevelData[x][z].GameObjects[GAMEOBJECT_CONST_INDEX_FIRE] = pEmitter;
			}
		}
	}
}
