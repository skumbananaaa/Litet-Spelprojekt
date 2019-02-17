#pragma once
#include <EnginePch.h>
#include <World/Grid.h>
#include <World/Tile.h>
#include <World/World.h>

struct tls {
	int g = 0;
	int f;
	glm::ivec3 parentTile;
	bool closed = false;
	bool stairsUp = false;
	bool stairsDown = false;
};

class Path
{
public:
	Path(const World* world);
	~Path();

	glm::ivec3* FindPath(const glm::ivec3& start, const glm::ivec3& goal);
	bool IsGoalSet();
	int GetNrOfPathTiles();

private:
	void AddToOpen(int x, int y, int z, int addX, int addY, int addZ);
	void CheckAdjacent();
	bool MoveToNextTile();

private:
	tls*** m_pppTiles;
	glm::ivec3* m_pOpenList;
	glm::ivec3* m_pPath;
	glm::ivec3 m_CurrentTile;
	glm::ivec3 m_GoalTile;
	glm::ivec3 m_StartTile;
	glm::ivec3 m_ClosestTile;
	glm::ivec2* m_pSize;
	const uint32* const** m_pppMap;
	const World* m_pWorld;
	int m_SmallestH;
	int m_NrOfTilesOpen;
	int m_NrOfPathTiles = 0;
	int m_LargestX = 0;
	int m_LargestZ = 0;
	bool m_GoalSet;
};

