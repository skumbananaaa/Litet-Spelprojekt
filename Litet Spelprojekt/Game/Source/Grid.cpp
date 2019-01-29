#include "..\Include\Grid.h"

Grid::Grid(const glm::ivec2 & size, const unsigned int ** tileVal, const glm::vec3 position)
{
	m_Size = size;
	m_pppTiles = new Tile**[size.x];

	for (int i = 0; i < size.x; i++)
	{
		m_pppTiles[i] = new Tile*[size.y];
		for (int j = 0; j < size.y; j++)
		{
			m_pppTiles[i][j] = new Tile(position + glm::vec3(i, 0.0f, j));
			m_pppTiles[i][j]->SetID(tileVal[i][j]);
		}
	}
}

Grid::Grid(const glm::ivec2 & size, const glm::vec3 position)
{
	m_Size = size;
	m_pppTiles = new Tile**[size.x];
	for (int i = 0; i < size.x; i++)
	{
		m_pppTiles[i] = new Tile*[size.y];
		for (int j = 0; j < size.y; j++)
		{
			m_pppTiles[i][j] = new Tile(position + glm::vec3(i, 0.0f, j));
			m_pppTiles[i][j]->SetID(0);
		}
	}
}

Grid::~Grid()
{
	for (int i = 0; i < m_Size.x; i++)
	{
		delete[] m_pppTiles[i];
	}
	delete[] m_pppTiles;
}

void Grid::edit(const glm::ivec2 & pos, const unsigned int & val)
{
	m_pppTiles[pos.x][pos.y]->SetID(val);
}

const int Grid::getVal(const glm::ivec2 & pos) const
{
	return m_pppTiles[pos.x][pos.y]->GetID();
}

Tile * Grid::GetTile(const glm::ivec2 & index)
{
	return m_pppTiles[index.x][index.y];
}

const glm::ivec2 Grid::GetSize() const
{
	return m_Size;
}

void Grid::SetColor(const glm::ivec2 & pos, const glm::vec4 & color)
{
	m_pppTiles[pos.x][pos.y]->SetColor(color);
}
