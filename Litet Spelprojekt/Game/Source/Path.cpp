#include "..\Include\Path.h"


Path::Path(int ** pMap, int sizeX, const int sizeY)
{
	this->m_ppMap = pMap;
	this->m_openList = new glm::ivec2[sizeX*sizeY];
	this->m_ppTiles = new tls[sizeX][sizeY];
}

Path::~Path()
{
}
