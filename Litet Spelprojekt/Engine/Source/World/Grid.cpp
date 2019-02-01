#include <EnginePch.h>
#include <World/Grid.h>

Grid::Grid(uint32 material, const glm::ivec2& size, uint32** ppTileVal, const glm::vec3& position)
{
	m_Size = size;
	m_pppTiles = new Tile**[size.x];

	for (int i = 0; i < size.x; i++)
	{
		m_pppTiles[i] = new Tile*[size.y];

		for (int j = 0; j < size.y; j++)
		{
			m_pppTiles[i][j] = new Tile(material, position + glm::vec3(i, 0.0f, j));
			m_pppTiles[i][j]->SetID(ppTileVal[i][j]);
		}
	}
}

Grid::Grid(uint32 material, const glm::ivec2& size, const glm::vec3& position)
{
	m_Size = size;
	m_pppTiles = new Tile**[size.x];

	for (int i = 0; i < size.x; i++)
	{
		m_pppTiles[i] = new Tile*[size.y];

		for (int j = 0; j < size.y; j++)
		{
			m_pppTiles[i][j] = new Tile(material, position + glm::vec3(i, 0.0f, j));
			m_pppTiles[i][j]->SetID(0);
		}
	}
}

Grid::~Grid()
{
	for (int i = 0; i < m_Size.x; i++)
	{
		delete[] m_pppTiles[i];
		m_pppTiles[i] = nullptr;
	}

	delete[] m_pppTiles;
	m_pppTiles = nullptr;
}

void Grid::Edit(const glm::ivec2& pos, uint32 val)
{
	m_pppTiles[pos.x][pos.y]->SetID(val);
}