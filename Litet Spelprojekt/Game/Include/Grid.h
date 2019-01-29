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

	void edit(const glm::ivec2 & pos, const unsigned int & val);
	const int getVal(const glm::ivec2 & pos) const;
	Tile * GetTile(const glm::ivec2 & index);
	const glm::ivec2 GetSize() const;

	void SetColor(const glm::ivec2 & pos, const glm::vec4 & color);
};
#endif