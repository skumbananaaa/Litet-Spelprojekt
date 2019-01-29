#ifndef GRID_H
#define GRID_H
#include "Tile.h"
class Grid
{
private:
	glm::ivec2 m_Size;
	glm::vec2 m_Pos;
	Tile *** m_pppTiles;
public:
	Grid(const glm::ivec2 & size, const unsigned int ** tileVal, const glm::vec3 position = glm::vec3(0.0f));
	Grid(const glm::ivec2 & size = glm::ivec2(0, 0), const glm::vec3 position = glm::vec3(0.0f));
	~Grid();
	///<summary>Edits the id value of the specified tile in the 2d grid.</summary>
	void edit(const glm::ivec2 & pos, const unsigned int & val);
	///<summary>Returns the id of the specified tile in the 2d grid.</summary>
	const unsigned int getVal(const glm::ivec2 & pos) const;
	///<summary>Returns a pointer to the selected tile.</summary>
	Tile * GetTile(const glm::ivec2 & pos);
	///<summary>Returns the size of the grid.</summary>
	const glm::ivec2 GetSize() const;

	///<summary>Sets the color of the specified tile to the specified color.</summary>
	void SetColor(const glm::ivec2 & pos, const glm::vec4 & color);
};
#endif