#include "..\Include\Tile.h"

Tile::Tile(const glm::vec3 & pos, const glm::vec4 & color, const unsigned int & id): m_pMesh(IndexedMesh::CreateQuad())
{
	GameObject::SetMesh(m_pMesh);
	m_pMaterial = new Material();
	m_pMaterial->SetColor(color);
	GameObject::SetMaterial(m_pMaterial);
	GameObject::SetRotation(glm::vec4(1.0f, 0.0f, 0.0f, -glm::pi<float>()/2.0f));
	GameObject::SetPosition(pos);
	GameObject::UpdateTransform();
}

Tile::~Tile()
{
	delete m_pMesh;
	delete m_pMaterial;
}

const unsigned int Tile::GetID() const
{
	return m_Id;
}

void Tile::SetID(const unsigned int & id)
{
	m_Id = id;
}

void Tile::SetColor(const glm::vec4 & color)
{
	m_pMaterial->SetColor(color);
}
