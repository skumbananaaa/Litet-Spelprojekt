#pragma once
#include <GLM\vec2.hpp>
#include <World/Grid.h>
#include <World/Tile.h>
#include <World/World.h>
#include <math.h>
#include <algorithm>

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
private:
	tls*** m_pppTiles;
	glm::ivec3* m_openList;
	glm::ivec3* m_pPath;
	glm::ivec3 m_currentTile;
	glm::ivec3 m_goalTile;
	glm::ivec3 m_startTile;
	glm::ivec3 m_closestTile;
	glm::ivec2* m_pSize;
	const uint32* const** m_pppMap;
	const World* m_pWorld;
	int m_smallestH;
	int m_nrOfTilesOpen;
	int m_nrOfPathTiles = 0;
	int m_largestX = 0;
	int m_largestZ = 0;
	bool m_goalSet;

private:
	void AddToOpen(int x, int y, int z, int addX, int addY, int addZ);
	void CheckAdjacent();
	bool MoveToNextTile();

public:
	Path(const World* world);
	~Path();

	glm::ivec3* FindPath(const glm::ivec3& start, const glm::ivec3& goal);
	bool IsGoalSet();
	int GetNrOfPathTiles();
};

