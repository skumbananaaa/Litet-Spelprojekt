#pragma once
#include <EnginePch.h>
#include <Graphics/GameObject.h>

class API Tile : public GameObject
{
public:
	Tile(const glm::vec3& pos = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec4& color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), uint32 id = 0);
	~Tile();

	///<summary>Sets the id of the tile.</summary>
	void SetID(uint32 id);
	///<summary>Sets the color of the tile.</summary>
	void SetColor(const glm::vec4& color);

	///<summary>Returns the id of the tile.</summary>
	const uint32 GetID() const;

private:
	uint32 m_Id;
	IndexedMesh* m_pMesh;
	Material* m_pMaterial;
};

inline const uint32 Tile::GetID() const
{
	return m_Id;
}