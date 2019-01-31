#include <EnginePch.h>
#include <World/Tile.h>

Tile::Tile(const glm::vec3& pos, const glm::vec4& color, uint32 id) : m_pMesh(IndexedMesh::CreateQuad())
{
	GameObject::SetMesh(m_pMesh);
	m_pMaterial = new Material();
	m_pMaterial->SetColor(color);
	GameObject::SetMaterial(m_pMaterial);
	GameObject::SetRotation(glm::vec4(1.0f, 0.0f, 0.0f, -glm::pi<float>() / 2.0f));
	GameObject::SetPosition(pos);
	GameObject::SetScale(glm::vec3(0.5, 0.5, 1));
	GameObject::UpdateTransform();
}

Tile::~Tile()
{
	Delete(m_pMesh);
	Delete(m_pMaterial);
}

void Tile::SetID(uint32 id)
{
	m_Id = id;
}

void Tile::SetColor(const glm::vec4 & color)
{
	m_pMaterial->SetColor(color);
}
