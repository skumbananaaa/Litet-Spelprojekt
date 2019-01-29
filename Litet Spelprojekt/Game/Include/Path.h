#pragma once
#include <GLM\vec2.hpp>
#include "..\Include\Grid.h"
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
	glm::ivec2 m_closestTile;
	int** m_ppMap;
	int m_smallestH;
	int m_nrOfTilesOpen;
	int m_nrOfPathTiles = 0;

private:
	void AddToOpen(int x, int y, int addX, int addY);
	void CheckAdjacent();
	bool MoveToNextTile();

public:
	Path(int** map, int sizeX, int sizeY);
	~Path();

	void FindPath(glm::ivec2 start, glm::ivec2 goal);

};

