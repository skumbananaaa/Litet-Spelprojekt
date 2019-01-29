#include "..\Include\Grid.h"

Grid::Grid(const glm::ivec2 & size, const int ** tiles)
{
	m_size = size;
	m_ppTiles = new int*[size.x];

	for (int i = 0; i < size.x; i++)
	{
		m_ppTiles[i] = new int[size.y];
		for (int j = 0; j < size.y; j++)
		{
			m_ppTiles[i][j] = tiles[i][j];
		}
	}

	m_pMesh = Mesh::CreateGrid(size.x, size.y, 1);
}

Grid::Grid(const glm::ivec2 & size)
{
	m_ppTiles = new int*[size.x];
	for (int i = 0; i < size.x; i++)
	{
		m_ppTiles[i] = new int[size.y];
		for (int j = 0; j < size.y; j++)
		{
			m_ppTiles[i][j] = 0;
		}
	}

	m_pMesh = Mesh::CreateGrid(size.x, size.y, 1);
}

Grid::~Grid()
{
	for (int i = 0; i < m_size.x; i++)
	{
		delete[] m_ppTiles[i];
	}
	delete[] m_ppTiles;
	delete m_pMesh;
}

void Grid::edit(const glm::ivec2 & pos, const int & val)
{
	m_ppTiles[pos.x][pos.y] = val;
}

const int Grid::getVal(const glm::ivec2 & pos) const
{
	return m_ppTiles[pos.x][pos.y];
}
