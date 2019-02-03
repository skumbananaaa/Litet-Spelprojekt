#include "..\Include\Path.h"


void Path::AddToOpen(int x, int y, int z, int addX, int addY, int addZ)
{
	int newY = std::min(std::max(y + addY, 0), ((int)m_pWorld->GetNumLevels() - 1));
	int newX = std::min(std::max(x + addX, 0), (m_pSize[newY].x - 1));
	int newZ = std::min(std::max(z + addZ, 0), (m_pSize[newY].y - 1));
	if (addY == 0 || (addY == 1 && m_pppTiles[x][y][z].stairsUp) || (addY == -1 && m_pppTiles[x][y][z].stairsDown)) {
		if (!(m_pppTiles[newX][newY][newZ].closed)) {
			if (m_pppTiles[newX][newY][newZ].g == 0 || m_pppTiles[newX][newY][newZ].g > m_pppTiles[x][y][z].g) {
				if ((m_pppMap[newY][newX][newZ] == m_pppMap[y][x][z] || m_pppMap[y][x][z] == 0 || m_pppMap[newY][newX][newZ] == 0 || addY != 0)  && m_pppMap[newY][newX][newZ] != 1) {
					m_openList[m_nrOfTilesOpen++] = glm::ivec3(newX, newY, newZ);
					m_pppTiles[newX][newY][newZ].parentTile = glm::ivec3(x, y, z);
					m_pppTiles[newX][newY][newZ].g = m_pppTiles[x][y][z].g + 1 + std::abs(addY);
					int h = std::abs(m_goalTile.x - newX) + std::abs(m_goalTile.y - newY) + std::abs(m_goalTile.z - newZ);
					m_pppTiles[newX][newY][newZ].f = m_pppTiles[newX][newY][newZ].g + h;
					if (h < m_smallestH && newY == m_goalTile.y) {
						m_smallestH = h;
						m_closestTile = glm::ivec3(newX, newY, newZ);
					}
				}
			}
		}
	}
}

void Path::CheckAdjacent()
{
	AddToOpen(m_currentTile.x, m_currentTile.y, m_currentTile.z, 1, 0, 0);
	AddToOpen(m_currentTile.x, m_currentTile.y, m_currentTile.z, -1, 0, 0);
	AddToOpen(m_currentTile.x, m_currentTile.y, m_currentTile.z, 0, 0, 1);
	AddToOpen(m_currentTile.x, m_currentTile.y, m_currentTile.z, 0, 0, -1);
	AddToOpen(m_currentTile.x, m_currentTile.y, m_currentTile.z, 0, 1, 0);
	AddToOpen(m_currentTile.x, m_currentTile.y, m_currentTile.z, 0, -1, 0);
}

bool Path::MoveToNextTile()
{
	if (m_nrOfTilesOpen != 0) {
		int index = m_nrOfTilesOpen - 1;
		int tempF = m_pppTiles[m_openList[index].x][m_openList[index].y][m_openList[index].z].f;
		for (int i = m_nrOfTilesOpen - 1; i >= 0; i--) {
			if ((m_pppTiles[m_openList[i].x][m_openList[i].y][m_openList[i].z].f < tempF) && !(m_pppTiles[m_openList[i].x][m_openList[i].y][m_openList[i].z].closed)) {
				tempF = m_pppTiles[m_openList[i].x][m_openList[i].y][m_openList[i].z].f;
				index = i;
			}
		}

		m_currentTile = glm::ivec3(m_openList[index].x, m_openList[index].y, m_openList[index].z);
		m_pppTiles[m_currentTile.x][m_currentTile.y][m_currentTile.z].closed = true;
		m_openList[index] = m_openList[--m_nrOfTilesOpen];

		CheckAdjacent();

		if (m_currentTile == m_goalTile) {
			return true;
		}
	}

	return false;
}

Path::Path(const World * world)
{
	m_pWorld = world;
	m_pppMap = new const uint32* const*[m_pWorld->GetNumLevels()];
	m_pSize = new glm::ivec2[m_pWorld->GetNumLevels()];
	int totalSize = 0;
	for (int i = 0; i < m_pWorld->GetNumLevels(); i++) {
		m_pSize[i].x = m_pWorld->GetLevel(i)->GetSizeX();
		m_pSize[i].y = m_pWorld->GetLevel(i)->GetSizeZ();
		m_pppMap[i] = m_pWorld->GetLevel(i)->GetLevel();
		totalSize += m_pSize[i].x * m_pSize[i].y;
		m_largestX = std::max(m_largestX, m_pSize[i].x);
		m_largestZ = std::max(m_largestZ, m_pSize[i].y);
	}
	m_pPath = new glm::ivec3[totalSize];
	m_openList = new glm::ivec3[totalSize];
	m_pppTiles = new tls**[m_largestX];
	for (int i = 0; i < m_largestX; i++) {
		m_pppTiles[i] = new tls*[m_pWorld->GetNumLevels()];
		for (int j = 0; j < m_pWorld->GetNumLevels(); j++) {
			m_pppTiles[i][j] = new tls[m_largestZ];
		}
	}

	int x, y, z;
	for (uint32 i = 0; i < m_pWorld->GetNumStairs(); i++) {
		x = m_pWorld->GetStairs()[i].x;
		y = m_pWorld->GetStairs()[i].y;
		z = m_pWorld->GetStairs()[i].z;
		m_pppTiles[x][y][z].stairsUp = true;
		m_pppTiles[x][y + 1][z].stairsDown = true;
	}
}

Path::~Path()
{
	for (int i = 0; i < m_largestX; i++) {
		for (int j = 0; j < m_pWorld->GetNumLevels(); j++) {
			delete[] m_pppTiles[i][j];
			m_pppTiles[i][j] = nullptr;
		}
		delete[] m_pppTiles[i];
		m_pppTiles[i] = nullptr;
	}

	delete[] m_pppTiles;
	delete[] m_pPath;
	delete[] m_openList;
	delete[] m_pppMap;
	delete[] m_pSize;

	m_pppTiles = nullptr;
	m_pppMap = nullptr;
	m_pPath = nullptr;
	m_openList = nullptr;
	m_pSize = nullptr;
}

glm::ivec3* Path::FindPath(const glm::ivec3& start, const glm::ivec3& goal)
{
	for (int i = 0; i < m_largestX; i++) {
		for (int j = 0; j < m_pWorld->GetNumLevels(); j++) {
			for (int k = 0; k < m_largestZ; k++) {
				m_pppTiles[i][j][k].g = 0;
				m_pppTiles[i][j][k].f = 0;
				m_pppTiles[i][j][k].closed = false;
			}
		}
	}

	m_nrOfTilesOpen = 0;
	m_nrOfPathTiles = 0;

	m_startTile = start;
	m_pppTiles[m_startTile.x][m_startTile.y][m_startTile.z].parentTile = m_startTile;
	m_currentTile = m_startTile;
	m_pppTiles[m_currentTile.x][m_currentTile.y][m_currentTile.z].closed = true;

	m_goalTile = goal;

	m_smallestH = std::abs(m_goalTile.x - m_startTile.x) + std::abs(m_goalTile.y - m_startTile.y) + std::abs(m_goalTile.z - m_startTile.z);
	m_closestTile = m_startTile;

	m_goalSet = true;

	CheckAdjacent();

	m_pppTiles[m_currentTile.x][m_currentTile.y][m_currentTile.z].closed = true;

	bool found = false;

	do {
		found = MoveToNextTile();
	} while (m_nrOfTilesOpen > 0 && !found);

	if (!found) {
		m_currentTile = m_closestTile;
	}

	do {
		m_pPath[m_nrOfPathTiles++] = m_currentTile;
		m_currentTile = m_pppTiles[m_currentTile.x][m_currentTile.y][m_currentTile.z].parentTile;
	} while (m_currentTile != m_startTile);

	m_goalSet = false;


	return m_pPath;
}

bool Path::IsGoalSet()
{
	return m_goalSet;
}

int Path::GetNrOfPathTiles()
{
	return m_nrOfPathTiles;
}

