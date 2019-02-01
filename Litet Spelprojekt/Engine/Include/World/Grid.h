#pragma once
#include <EnginePch.h>
#include <World/Tile.h>

class API Grid
{
public:
	Grid(const glm::uvec2& size, uint32** ppTileVal, const glm::vec3& position = glm::vec3(0.0f));
	Grid(const glm::uvec2& size = glm::ivec2(0, 0), const glm::vec3& position = glm::vec3(0.0f));
	~Grid();

	///<summary>Edits the id value of the specified tile in the 2d grid.</summary>
	void Edit(const glm::uvec2& pos, uint32 val);

	///<summary>Returns the id of the specified tile in the 2d grid.</summary>
	const unsigned int GetVal(const glm::uvec2& pos) const;
	///<summary>Returns a pointer to the selected tile.</summary>
	Tile* GetTile(const glm::ivec2& pos);
	///<summary>Returns the size of the grid.</summary>
	const glm::uvec2& GetSize() const;
	///<summary>Returns all the pointers to tiles in a double pointer array.</summary>
	const Tile* const * const * GetGrid() const;

	///<summary>Sets the color of the specified tile to the specified color.</summary>
	void SetColor(const glm::uvec2& pos, const glm::vec4& color);

private:
	glm::uvec2 m_Size;
	glm::vec2 m_Pos;
	Tile*** m_pppTiles;
};

inline const unsigned int Grid::GetVal(const glm::uvec2& pos) const
{
	return m_pppTiles[pos.x][pos.y]->GetID();
}

inline Tile* Grid::GetTile(const glm::ivec2& pos)
{
	return m_pppTiles[pos.x][pos.y];
}

inline const glm::uvec2& Grid::GetSize() const
{
	return m_Size;
}

inline const Tile* const * const * Grid::GetGrid() const
{
	return m_pppTiles;
}