#include <EnginePch.h>
#include <World/WorldLevel.h>

WorldLevel::WorldLevel(const uint32* const levelIndexes, uint32 sizeX, uint32 sizeZ) noexcept
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

const TileData * const * const WorldLevel::GetLevelData() const noexcept
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

const glm::vec4 & WorldLevel::GetWall(uint32 index) const noexcept
{
	assert(index < m_Walls.size());
	return m_Walls[index];
}

uint32 WorldLevel::GetNrOfWalls() const noexcept
{
	return m_Walls.size();
}

void WorldLevel::GenerateWalls()
{
	bool wallH = false, wallV = false;
	glm::vec2 startWallH(0, 0), endWallH(0, 0);
	glm::vec2 startWallV(0, 0), endWallV(0, 0);

	for (int i = 0; i < m_SizeX - 1; i++) {
		for (int j = 0; j < m_SizeZ; j++) {
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
	for (int i = 0; i < m_SizeZ - 1; i++) {
		for (int j = 0; j < m_SizeX; j++) {
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

void WorldLevel::UpdateFire(float dt)
{

	for (uint32 x = 0; x < m_SizeX; x++)
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
	}
}
