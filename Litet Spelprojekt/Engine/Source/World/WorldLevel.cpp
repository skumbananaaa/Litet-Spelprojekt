#include <EnginePch.h>
#include <World/WorldLevel.h>

WorldLevel::WorldLevel(const uint32* const levelIndexes, uint32 sizeX, uint32 sizeZ) noexcept
{
	m_SizeX = sizeX;
	m_SizeZ = sizeZ;
	m_ppLevel = new uint32*[sizeX];
	for (uint32 x = 0; x < m_SizeX; x++)
	{
		m_ppLevel[x] = new uint32[sizeZ];

		for (uint32 z = 0; z < m_SizeZ; z++)
		{
			m_ppLevel[x][z] = levelIndexes[x * m_SizeZ + z];
		}
	}
}

WorldLevel::~WorldLevel()
{
	for (uint32 x = 0; x < m_SizeX; x++)
	{
		delete[] m_ppLevel[x];
		m_ppLevel[x] = nullptr;
	}

	delete[] m_ppLevel;
	m_ppLevel = nullptr;
}

const uint32* const* const WorldLevel::GetLevel() const noexcept
{
	return m_ppLevel;
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

const glm::ivec2 & WorldLevel::GetStairsUp(uint32 index) const noexcept
{
	assert(index < m_StairsUp.size());
	return m_StairsUp[index];
}

uint32 WorldLevel::GetNrOfStairsUp() const noexcept
{
	return m_StairsUp.size();
}

const glm::ivec2 & WorldLevel::GetStairsDown(uint32 index) const noexcept
{
	assert(index < m_StairsDown.size());
	return m_StairsDown[index];
}

uint32 WorldLevel::GetNrOfStairsDown() const noexcept
{
	return m_StairsDown.size();
}

void WorldLevel::GenerateWalls()
{
	bool wallH = false, wallV = false;
	glm::vec2 startWallH(0, 0), endWallH(0, 0);
	glm::vec2 startWallV(0, 0), endWallV(0, 0);

	for (int i = 0; i < m_SizeX - 1; i++) {
		for (int j = 0; j < m_SizeZ; j++) {
			wallH = (m_ppLevel[i][j] != m_ppLevel[i + 1][j]);
			if ((!wallH || ((m_ppLevel[i][j] == 0 || m_ppLevel[i][j] == 9 || m_ppLevel[i][j] == 8) && m_ppLevel[i + 1][j] != 1) || ((m_ppLevel[i + 1][j] == 0 || m_ppLevel[i + 1][j] == 9 || m_ppLevel[i + 1][j] == 8) && m_ppLevel[i][j] != 1) || m_ppLevel[i][j] != m_ppLevel[i][j - 1] || m_ppLevel[i + 1][j] != m_ppLevel[i + 1][j - 1]) && startWallH != glm::vec2(0, 0)) {
				endWallH = glm::vec2(i + 0.5, j - 0.5);
				m_Walls.push_back(glm::vec4((startWallH + endWallH) / 2.0f, endWallH - startWallH));
				startWallH = glm::vec2(0, 0);
			}
			if (wallH && startWallH == glm::vec2(0, 0) && ((m_ppLevel[i][j] != 0 && m_ppLevel[i][j] != 9 && m_ppLevel[i][j] != 8) || m_ppLevel[i + 1][j] == 1) && ((m_ppLevel[i + 1][j] != 0 && m_ppLevel[i + 1][j] != 9 && m_ppLevel[i + 1][j] != 8) || m_ppLevel[i][j] == 1)) {
				startWallH = glm::vec2(i + 0.5, j - 0.5);
			}

			if (m_ppLevel[i][j] == 9) {
				startWallH = glm::vec2(i + 0.5, j - 0.5);
				endWallH = glm::vec2(i + 0.5, j + 0.5);
				m_StairsUp.push_back(glm::ivec2(i, j));
			}
			else if (m_ppLevel[i][j] == 8) {
				m_StairsDown.push_back(glm::ivec2(i, j));
			}
		}
	}
	for (int i = 0; i < m_SizeZ - 1; i++) {
		for (int j = 0; j < m_SizeX; j++) {
			wallV = (m_ppLevel[j][i] != m_ppLevel[j][i + 1]);
			if ((!wallV || ((m_ppLevel[j][i] == 0 || m_ppLevel[j][i] == 9 || m_ppLevel[j][i] == 8) && m_ppLevel[j][i + 1] != 1) || ((m_ppLevel[j][i + 1] == 0 || m_ppLevel[j][i + 1] == 9 || m_ppLevel[j][i + 1] == 8) && m_ppLevel[j][i] != 1) || m_ppLevel[j][i] != m_ppLevel[j - 1][i] || (m_ppLevel[j][i + 1] != m_ppLevel[j - 1][i + 1])) && startWallV != glm::vec2(0, 0)) {
				endWallV = glm::vec2(j - 0.5, i + 0.5);
				m_Walls.push_back(glm::vec4((startWallV + endWallV) / 2.0f, endWallV - startWallV));
				startWallV = glm::vec2(0, 0);
			}
			if (wallV && startWallV == glm::vec2(0, 0) && ((m_ppLevel[j][i] != 0 && m_ppLevel[j][i] != 9 && m_ppLevel[j][i] != 8) || m_ppLevel[j][i + 1] == 1) && ((m_ppLevel[j][i + 1] != 0 && m_ppLevel[j][i + 1] != 9 && m_ppLevel[j][i + 1] != 8) || m_ppLevel[j][i] == 1)) {
				startWallV = glm::vec2(j - 0.5, i + 0.5);
			}
		}
	}
}