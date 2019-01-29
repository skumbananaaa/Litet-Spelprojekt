#ifndef TILE_H
#define TILE_H
#include <Graphics/GameObject.h>
class Tile: public GameObject
{
private:
	unsigned int m_Id;
	IndexedMesh * m_pMesh;
	Material * m_pMaterial;
public:
	Tile(const glm::vec3 & pos = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec4 & color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), const unsigned int & id = 0);
	~Tile();
	const unsigned int GetID() const;
	void SetID(const unsigned int & id);
	void SetColor(const glm::vec4 & color);
};

#endif