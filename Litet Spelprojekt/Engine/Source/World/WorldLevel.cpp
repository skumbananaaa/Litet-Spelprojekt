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
			m_ppLevelData[x][z].BurnsAt = 100;
			m_ppLevelData[x][z].Temp = 30;
			m_ppLevelData[x][z].WaterLevel = 0.0f;
			m_ppLevelData[x][z].Burning = false;
			m_ppLevelData[x][z].WaterBlockName = "WaterBlock [" + std::to_string(x) + ", " + std::to_string(levelHeight) + ", " + std::to_string(z) + "]";
		}
	}
}

WorldLevel::~WorldLevel()
{
	for (uint32 x = 0; x < m_SizeX; x++)
	{
		DeleteArr(m_ppLevelData[x]);
		delete[] m_ppLevel[x];
		m_ppLevel[x] = nullptr;
	}
	Delete(m_ppLevelData);
	delete[] m_ppLevel;
	m_ppLevel = nullptr;
}

const uint32* const* const WorldLevel::GetLevel() const noexcept
{
	return m_ppLevel;
}

TileData* const * const WorldLevel::GetLevelData() noexcept
{
	return m_ppLevelData;
}


const TileData* const * const WorldLevel::GetLevelData() const noexcept
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
	return m_Walls.size();
}

const std::vector<glm::uvec4>& WorldLevel::GetRooms() const noexcept
{
	return roomBounds;
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
			if ((!wallH || (m_ppLevel[i][j] == 0 && m_ppLevel[i + 1][j] != 1) || (m_ppLevel[i + 1][j] == 0 && m_ppLevel[i][j] != 1) || m_ppLevel[i][j] != m_ppLevel[i][j - 1] || m_ppLevel[i + 1][j] != m_ppLevel[i + 1][j - 1]) && startWallH != glm::vec2(0, 0))
			{
				endWallH = glm::vec2(i + 0.5, j - 0.5);
				m_Walls.push_back(glm::vec4((startWallH + endWallH) / 2.0f, endWallH - startWallH));
				startWallH = glm::vec2(0, 0);
			}
			if (wallH && startWallH == glm::vec2(0, 0) && (m_ppLevel[i][j] != 0 || m_ppLevel[i + 1][j] == 1) && (m_ppLevel[i + 1][j] != 0 || m_ppLevel[i][j] == 1))
			{
				startWallH = glm::vec2(i + 0.5, j - 0.5);
			}
		}
	}

	for (uint32 i = roomBounds.size(); i <= maxRoomNum; i++)
	{
		roomBounds.push_back(glm::uvec4(11, 0, 41, 0));
	}

	for (uint32 i = 0; i < m_SizeZ - 1; i++) {
		for (uint32 j = 0; j < m_SizeX; j++) {

			roomBounds[m_ppLevel[j][i]].x = glm::min(roomBounds[m_ppLevel[j][i]].x, j);
			roomBounds[m_ppLevel[j][i]].y = glm::max(roomBounds[m_ppLevel[j][i]].y, j);
			roomBounds[m_ppLevel[j][i]].z = glm::min(roomBounds[m_ppLevel[j][i]].z, i);
			roomBounds[m_ppLevel[j][i]].w = glm::max(roomBounds[m_ppLevel[j][i]].w, i);

			wallV = (m_ppLevel[j][i] != m_ppLevel[j][i + 1]);
			if ((!wallV || (m_ppLevel[j][i] == 0 && m_ppLevel[j][i + 1] != 1) || (m_ppLevel[j][i + 1] == 0 && m_ppLevel[j][i] != 1) || m_ppLevel[j][i] != m_ppLevel[j - 1][i] || (m_ppLevel[j][i + 1] != m_ppLevel[j - 1][i + 1])) && startWallV != glm::vec2(0, 0))
			{
				endWallV = glm::vec2(j - 0.5, i + 0.5);
				m_Walls.push_back(glm::vec4((startWallV + endWallV) / 2.0f, endWallV - startWallV));
				startWallV = glm::vec2(0, 0);
			}
			if (wallV && startWallV == glm::vec2(0, 0) && (m_ppLevel[j][i] != 0 || m_ppLevel[j][i + 1] == 1) && (m_ppLevel[j][i + 1] != 0 || m_ppLevel[j][i] == 1))
			{
				startWallV = glm::vec2(j - 0.5, i + 0.5);
			}
		}
	}
}
void WorldLevel::GenerateWater(Scene* scene, uint32 levelHeight)
{
	GameObject* pGameObject = nullptr;

	for (uint32 x = 0; x < m_SizeX; x++)
	{
		for (uint32 z = 0; z < m_SizeZ; z++)
		{
			pGameObject = new GameObject();
			pGameObject->SetIsReflectable(true);
			pGameObject->SetIsVisible(false);
			pGameObject->SetMesh(MESH::CUBE);
			pGameObject->SetMaterial(MATERIAL::WATER);
			pGameObject->SetPosition(glm::vec3(x + 0.5f, levelHeight + 0.5f, z + 0.5f));
			pGameObject->UpdateTransform();
			pGameObject->SetName(m_ppLevelData[x][z].WaterBlockName);
			scene->AddGameObject(pGameObject);
		}
	}
}

void WorldLevel::SetTileData(const glm::ivec2 & pos, const TileData & data)
{
	m_ppLevelData[pos.x][pos.y] = data;
}

