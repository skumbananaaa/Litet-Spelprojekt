#include <EnginePch.h>
#include "..\Include\Path.h"

void Path::AddToOpen(int x, int y, int z, int addX, int addY, int addZ)
{
	int newY = std::min(std::max(y + addY, 0), ((int)m_pWorld->GetNumLevels() / 2 - 1));
	int newX = std::min(std::max(x + addX, 0), (m_pSize[newY].x - 1));
	int newZ = std::min(std::max(z + addZ, 0), (m_pSize[newY].y - 1));
	if (addY == 0 || (addY == 1 && m_pppTiles[x][y][z].stairsUp) || (addY == -1 && m_pppTiles[x][y][z].stairsDown))
	{
		if (!(m_pppTiles[newX][newY][newZ].closed))
		{
			if (m_pppTiles[newX][newY][newZ].g == 0 || m_pppTiles[newX][newY][newZ].g > m_pppTiles[x][y][z].g)
			{
				if ((m_pppMap[newY][newX][newZ] == m_pppMap[y][x][z] || (m_pWorld->GetLevel(y * 2)->GetLevelData()[x][z].HasDoor && m_pWorld->GetLevel(newY * 2)->GetLevelData()[newX][newZ].HasDoor) || addY != 0)  && m_pppMap[newY][newX][newZ] != 1)
				{
					m_pOpenList[m_NrOfTilesOpen++] = glm::ivec3(newX, newY, newZ);
					m_pppTiles[newX][newY][newZ].parentTile = glm::ivec3(x, y, z);
					m_pppTiles[newX][newY][newZ].g = m_pppTiles[x][y][z].g + 1 + std::abs(addY);
					
					int h = std::abs(m_GoalTile.x - newX) + std::abs(m_GoalTile.y - newY) + std::abs(m_GoalTile.z - newZ);
					m_pppTiles[newX][newY][newZ].f = m_pppTiles[newX][newY][newZ].g + h;
					
					if (h < m_SmallestH && newY == m_GoalTile.y)
					{
						m_SmallestH = h;
						m_ClosestTile = glm::ivec3(newX, newY, newZ);
					}
				}
			}
		}
	}
}

void Path::CheckAdjacent()
{
	AddToOpen(m_CurrentTile.x, m_CurrentTile.y, m_CurrentTile.z, 1, 0, 0);
	AddToOpen(m_CurrentTile.x, m_CurrentTile.y, m_CurrentTile.z, -1, 0, 0);
	AddToOpen(m_CurrentTile.x, m_CurrentTile.y, m_CurrentTile.z, 0, 0, 1);
	AddToOpen(m_CurrentTile.x, m_CurrentTile.y, m_CurrentTile.z, 0, 0, -1);
	AddToOpen(m_CurrentTile.x, m_CurrentTile.y, m_CurrentTile.z, 0, 1, 0);
	AddToOpen(m_CurrentTile.x, m_CurrentTile.y, m_CurrentTile.z, 0, -1, 0);
}

bool Path::MoveToNextTile()
{
	if (m_NrOfTilesOpen != 0)
	{
		int index = m_NrOfTilesOpen - 1;
		int tempF = m_pppTiles[m_pOpenList[index].x][m_pOpenList[index].y][m_pOpenList[index].z].f;
		for (int i = m_NrOfTilesOpen - 1; i >= 0; i--)
		{
			if ((m_pppTiles[m_pOpenList[i].x][m_pOpenList[i].y][m_pOpenList[i].z].f < tempF) && !(m_pppTiles[m_pOpenList[i].x][m_pOpenList[i].y][m_pOpenList[i].z].closed))
			{
				tempF = m_pppTiles[m_pOpenList[i].x][m_pOpenList[i].y][m_pOpenList[i].z].f;
				index = i;
			}
		}

		m_CurrentTile = m_pOpenList[index];
		m_pppTiles[m_CurrentTile.x][m_CurrentTile.y][m_CurrentTile.z].closed = true;
		m_pOpenList[index] = m_pOpenList[--m_NrOfTilesOpen];

		CheckAdjacent();

		if (m_CurrentTile == m_GoalTile)
		{
			return true;
		}
	}

	return false;
}

Path::Path(const World * world)
{
	m_pWorld = world;
	m_pppMap = new const uint32* const*[m_pWorld->GetNumLevels() / 2];
	m_pSize = new glm::ivec2[m_pWorld->GetNumLevels() / 2];
	
	int totalSize = 0;
	for (int i = 0; i < (m_pWorld->GetNumLevels() / 2); i++)
	{
		m_pSize[i].x = m_pWorld->GetLevel(i * 2)->GetSizeX();
		m_pSize[i].y = m_pWorld->GetLevel(i * 2)->GetSizeZ();
		m_pppMap[i] = m_pWorld->GetLevel(i * 2)->GetLevel();
		totalSize += m_pSize[i].x * m_pSize[i].y;
		m_LargestX = std::max(m_LargestX, m_pSize[i].x);
		m_LargestZ = std::max(m_LargestZ, m_pSize[i].y);
	}
	
	m_pPath = new glm::ivec3[totalSize];
	m_pOpenList = new glm::ivec3[totalSize];

	m_pppTiles = new tls**[m_LargestX];
	for (int i = 0; i < m_LargestX; i++)
	{
		m_pppTiles[i] = new tls*[m_pWorld->GetNumLevels() / 2];
		for (int j = 0; j < m_pWorld->GetNumLevels() / 2; j++)
		{
			m_pppTiles[i][j] = new tls[m_LargestZ];
		}
	}

	int x, y, z;
	for (uint32 i = 0; i < m_pWorld->GetNumStairs(); i++)
	{
		x = m_pWorld->GetStairs()[i].x;
		y = m_pWorld->GetStairs()[i].y / 2;
		z = m_pWorld->GetStairs()[i].z;
		m_pppTiles[x][y][z].stairsUp = true;
		m_pppTiles[x][y + 1][z].stairsDown = true;
	}
}

Path::~Path()
{
	for (int i = 0; i < m_LargestX; i++)
	{
		for (int j = 0; j < m_pWorld->GetNumLevels() / 2; j++)
		{
			DeleteArrSafe(m_pppTiles[i][j]);
		}

		DeleteArrSafe(m_pppTiles[i]);
	}
	DeleteArrSafe(m_pppTiles);

	DeleteSafe(m_pPath);
	DeleteSafe(m_pOpenList);
	DeleteSafe(m_pppMap);
	DeleteSafe(m_pSize);
}

glm::ivec3* Path::FindPath(const glm::ivec3& start, const glm::ivec3& goal)
{
	for (int i = 0; i < m_LargestX; i++)
	{
		for (int j = 0; j < m_pWorld->GetNumLevels() / 2; j++)
		{
			for (int k = 0; k < m_LargestZ; k++)
			{
				m_pppTiles[i][j][k].g = 0;
				m_pppTiles[i][j][k].f = 0;
				m_pppTiles[i][j][k].closed = false;
			}
		}
	}

	m_NrOfTilesOpen = 0;
	m_NrOfPathTiles = 0;

	m_StartTile = start;
	m_pppTiles[m_StartTile.x][m_StartTile.y][m_StartTile.z].parentTile = m_StartTile;
	m_CurrentTile = m_StartTile;
	m_pppTiles[m_CurrentTile.x][m_CurrentTile.y][m_CurrentTile.z].closed = true;

	m_GoalTile = goal;

	m_SmallestH = std::abs(m_GoalTile.x - m_StartTile.x) + std::abs(m_GoalTile.y - m_StartTile.y) + std::abs(m_GoalTile.z - m_StartTile.z);
	m_ClosestTile = m_StartTile;

	m_GoalSet = true;

	CheckAdjacent();

	m_pppTiles[m_CurrentTile.x][m_CurrentTile.y][m_CurrentTile.z].closed = true;

	bool found = false;

	do
	{
		found = MoveToNextTile();
	} while (m_NrOfTilesOpen > 0 && !found);

	if (!found)
	{
		m_CurrentTile = m_ClosestTile;
	}

	do
	{
		m_pPath[m_NrOfPathTiles++] = m_CurrentTile;
		m_CurrentTile = m_pppTiles[m_CurrentTile.x][m_CurrentTile.y][m_CurrentTile.z].parentTile;
	} while (m_CurrentTile != m_StartTile);

	m_GoalSet = false;


	return m_pPath;
}

bool Path::IsGoalSet()
{
	return m_GoalSet;
}

int Path::GetNrOfPathTiles()
{
	return m_NrOfPathTiles;
}

const World * Path::GetWorld() const noexcept
{
	return m_pWorld;
}

