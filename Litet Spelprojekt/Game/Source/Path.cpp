#include "..\Include\Path.h"


void Path::AddToOpen(int x, int y, int addX, int addY)
{
	int newX = std::min(std::max(x + addX, 0), (m_size.x - 1));
	int newY = std::min(std::max(y + addY, 0), (m_size.y - 1));
	if (!(m_ppTiles[newX][newY].m_closed)) {
		if (m_ppTiles[newX][newY].m_g == 0 || m_ppTiles[newX][newY].m_g > m_ppTiles[x][y].m_g) {
			if ((m_ppMap[newX][newY] == m_ppMap[x][y] || m_ppMap[x][y] == 0 || m_ppMap[newX][newY] == 0 || m_ppMap[x][y] == 9 || m_ppMap[newX][newY] == 9 || m_ppMap[x][y] == 8) && m_ppMap[newX][newY] != 1) {
				m_openList[m_nrOfTilesOpen++] = glm::ivec2(newX, newY);
				m_ppTiles[newX][newY].m_parentTile = glm::ivec2(x, y);
				m_ppTiles[newX][newY].m_g = m_ppTiles[x][y].m_g + 1;
				int h = std::abs(m_targetTile.x - newX) + std::abs(m_targetTile.y - newY);
				m_ppTiles[newX][newY].m_f = m_ppTiles[newX][newY].m_g + h;
				if (h < m_smallestH) {
					m_smallestH = h;
					m_closestTile = glm::ivec2(newX, newY);
				}
			}
		}
	}
}

void Path::CheckAdjacent()
{
	AddToOpen(m_currentTile.x, m_currentTile.y, 1, 0);
	AddToOpen(m_currentTile.x, m_currentTile.y, -1, 0);
	AddToOpen(m_currentTile.x, m_currentTile.y, 0, 1);
	AddToOpen(m_currentTile.x, m_currentTile.y, 0, -1);
}

bool Path::MoveToNextTile()
{
	if (m_nrOfTilesOpen != 0) {
		int index = m_nrOfTilesOpen - 1;
		int tempF = m_ppTiles[m_openList[index].x][m_openList[index].y].m_f;
		for (int i = m_nrOfTilesOpen - 1; i >= 0; i--) {
			if ((m_ppTiles[m_openList[i].x][m_openList[i].y].m_f < tempF) && !(m_ppTiles[m_openList[i].x][m_openList[i].y].m_closed)) {
				tempF = m_ppTiles[m_openList[i].x][m_openList[i].y].m_f;
				index = i;
			}
		}

		m_currentTile = glm::ivec2(m_openList[index].x, m_openList[index].y);
		m_ppTiles[m_currentTile.x][m_currentTile.y].m_closed = true;
		m_openList[index] = m_openList[--m_nrOfTilesOpen];

		CheckAdjacent();

		if (m_currentTile == m_targetTile) {
			return true;
		}
	}

	return false;
}

Path::Path(const Tile* const * const * pppTilemap, const glm::ivec2& size)
{
	m_size = size;
	//m_ppMap = map;
	m_pPath = new glm::ivec2[size.x * size.y];
	m_openList = new glm::ivec2[m_size.x * m_size.y];
}

Path::Path(const World * world, uint32 level)
{
	m_pWorld = world;
	this->SetLevel(level);
	m_pPath = new glm::ivec2[m_size.x * m_size.y];
	m_openList = new glm::ivec2[m_size.x * m_size.y];
}

Path::~Path()
{
	delete[] m_pPath;
	delete[] m_openList;
	m_ppMap = nullptr;
	m_pPath = nullptr;
	m_openList = nullptr;
}

glm::ivec2* Path::FindPath(const glm::ivec3& start, const glm::ivec3& goal)
{
	m_ppTiles = new tls*[m_size.x];
	for (int i = 0; i < m_size.x; i++) {
		m_ppTiles[i] = new tls[m_size.y];
	}

	m_nrOfTilesOpen = 0;
	m_nrOfPathTiles = 0;
	m_stairTile = glm::ivec2(0, 0);

	m_startTile = glm::ivec2(start.x, start.z);
	m_ppTiles[m_startTile.x][m_startTile.y].m_parentTile = start;
	m_currentTile = m_startTile;
	m_ppTiles[m_currentTile.x][m_currentTile.y].m_closed = true;

	if (goal.y > start.y) {
		m_stairTile = this->FindStairsUp(start.y);
	}
	else if (goal.y < start.y) {
		m_stairTile = this->FindStairsDown(start.y);
	}
	else {
		m_stairTile = glm::ivec2(0, 0);
	}

	m_goalTile = glm::ivec2(goal.x, goal.z);

	if (m_stairTile == glm::ivec2(0, 0)) {
		m_targetTile = m_goalTile;
	}
	else {
		m_targetTile = m_stairTile;
	}

	m_smallestH = std::abs(m_targetTile.x - m_startTile.x) + std::abs(m_targetTile.y - m_startTile.y);
	m_closestTile = m_startTile;

	m_goalSet = true;

	CheckAdjacent();

	m_ppTiles[m_currentTile.x][m_currentTile.y].m_closed = true;

	bool found = false;

	do {
		found = MoveToNextTile();
	} while (m_nrOfTilesOpen > 0 && !found);

	if (!found) {
		m_currentTile = m_closestTile;
	}

	do {
		m_pPath[m_nrOfPathTiles++] = m_currentTile;
		m_currentTile = m_ppTiles[m_currentTile.x][m_currentTile.y].m_parentTile;
	} while (m_currentTile != m_startTile);

	m_goalSet = false;

	 for (int i = 0; i < m_size.x; i++) {
		 delete[] m_ppTiles[i];
		 m_ppTiles[i] = nullptr;
	 }
	 delete[] m_ppTiles;
	 m_ppTiles = nullptr;

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

void Path::SetLevel(uint32 newLevel)
{
	m_size = glm::ivec2(m_pWorld->GetLevel(newLevel)->GetSizeX(), m_pWorld->GetLevel(newLevel)->GetSizeZ());
	m_ppMap = m_pWorld->GetLevel(newLevel)->GetLevel();
}
