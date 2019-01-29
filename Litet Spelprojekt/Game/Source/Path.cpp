#include "..\Include\Path.h"


void Path::AddToOpen(int x, int y, int addX, int addY)
{
	int newX = x + addX;
	int newY = y + addY;
	if (!(this->m_ppTiles[newX][newY].m_closed)) {
		if (this->m_ppTiles[newX][newY].m_g == 0 || this->m_ppTiles[newX][newY].m_g > this->m_ppTiles[x][y].m_g) {
			if ((this->m_ppMap[newX][newY] == this->m_ppMap[x][y] || this->m_ppMap[x][y] == 0 || this->m_ppMap[newX][newY] == 0) && this->m_ppMap[newX][newY] != 1) {
				this->m_openList[this->m_nrOfTilesOpen++] = glm::ivec2(newX, newY);
				this->m_ppTiles[newX][newY].m_parentTile = glm::ivec2(x, y);
				this->m_ppTiles[newX][newY].m_g = this->m_ppTiles[x][y].m_g + 1;
				int h = std::abs(this->m_goalTile.x - newX) + std::abs(this->m_goalTile.y - newY);
				this->m_ppTiles[newX][newY].m_f = this->m_ppTiles[newX][newY].m_g + h;
				if (h < this->m_smallestH) {
					this->m_smallestH = h;
					this->m_closestTile = glm::ivec2(newX, newY);
				}
			}
		}
	}
}

void Path::CheckAdjacent()
{
	AddToOpen(this->m_currentTile.x, this->m_currentTile.y, 1, 0);
	AddToOpen(this->m_currentTile.x, this->m_currentTile.y, -1, 0);
	AddToOpen(this->m_currentTile.x, this->m_currentTile.y, 0, 1);
	AddToOpen(this->m_currentTile.x, this->m_currentTile.y, 0, -1);
}

bool Path::MoveToNextTile()
{
	if (this->m_nrOfTilesOpen != 0) {
		int index = this->m_nrOfTilesOpen - 1;
		int tempF = this->m_ppTiles[this->m_openList[index].x][this->m_openList[index].y].m_f;
		for (int i = this->m_nrOfTilesOpen - 1; i >= 0; i--) {
			if ((this->m_ppTiles[this->m_openList[i].x][this->m_openList[i].y].m_f < tempF) && !(this->m_ppTiles[this->m_openList[i].x][this->m_openList[i].y].m_closed)) {
				tempF = this->m_ppTiles[this->m_openList[i].x][this->m_openList[i].y].m_f;
				index = i;
			}
		}

		this->m_currentTile = glm::ivec2(this->m_openList[index].x, this->m_openList[index].y);
		this->m_ppTiles[this->m_currentTile.x][this->m_currentTile.y].m_closed = true;
		this->m_openList[index] = this->m_openList[--this->m_nrOfTilesOpen];

		CheckAdjacent();

		if (this->m_currentTile == this->m_goalTile) {
			return true;
		}
	}

	return false;
}

Path::Path(int ** pMap, int sizeX, const int sizeY)
{
	this->m_ppMap = pMap;
	this->m_openList = new glm::ivec2[sizeX*sizeY];
	this->m_pPath = new glm::ivec2[sizeX*sizeY];
	this->m_ppTiles = new tls*[sizeX];
	for (int i = 0; i < sizeX; i++) {
		this->m_ppTiles[i] = new tls[sizeY];
	}
}

Path::~Path()
{
}

void Path::FindPath(glm::ivec2 start, glm::ivec2 goal)
{
	this->m_startTile = start;
	this->m_ppTiles[this->m_startTile.x][this->m_startTile.y].m_parentTile = start;
	this->m_currentTile = this->m_startTile;
	this->m_ppTiles[this->m_currentTile.x][this->m_currentTile.y].m_closed = true;

	/*int x = std::max(std::min(goal.y / 35, 19), 0);
	int y = std::max(std::min(goal.x / 35, 19), 0);
	this->m_goalTile = glm::ivec2(x, y);*/
	this->m_goalTile = goal;
	this->m_smallestH = std::abs(this->m_goalTile.x - this->m_startTile.x) + std::abs(this->m_goalTile.y - this->m_startTile.y);
	this->m_closestTile = this->m_startTile;

	CheckAdjacent();

	this->m_ppTiles[this->m_currentTile.x][this->m_currentTile.y].m_closed = true;

	bool found = false;

	do {
		found = MoveToNextTile();
	} while (this->m_nrOfTilesOpen > 0 && !found);

	if (!found) {
		this->m_currentTile = this->m_closestTile;
	}

	do {
		this->m_pPath[this->m_nrOfPathTiles++] = this->m_currentTile;
		this->m_currentTile = this->m_ppTiles[this->m_currentTile.x][this->m_currentTile.y].m_parentTile;
	} while (this->m_currentTile != this->m_startTile);
}
