#pragma once
#include <GLM\vec2.hpp>
#include <World/Grid.h>
#include <World/Tile.h>
#include <World/World.h>
#include <math.h>
#include <algorithm>

struct tls {
	float m_g = 0;
	float m_f;
	glm::ivec2 m_parentTile;
	bool m_closed = false;
};

class Path
{
private:
	tls** m_ppTiles;
	glm::ivec2* m_openList;
	glm::ivec2* m_pPath;
	glm::ivec2 m_currentTile;
	glm::ivec2 m_goalTile;
	glm::ivec2 m_startTile;
	glm::ivec2 m_stairTile;
	glm::ivec2 m_targetTile;
	glm::ivec2 m_closestTile;
	glm::ivec2 m_size;
	const uint32* const* m_ppMap;
	const World* m_pWorld;
	int m_smallestH;
	int m_nrOfTilesOpen;
	int m_nrOfPathTiles = 0;
	bool m_goalSet;

private:
	void AddToOpen(int x, int y, int addX, int addY);
	void CheckAdjacent();
	bool MoveToNextTile();
	const glm::ivec2& FindStairsUp(uint32 level);
	const glm::ivec2& FindStairsDown(uint32 level);

public:
	Path(const World* world, uint32 level);
	~Path();

	glm::ivec2* FindPath(const glm::ivec3& start, const glm::ivec3& goal);
	bool IsGoalSet();
	int GetNrOfPathTiles();
	void SetLevel(uint32 newLevel);
};

