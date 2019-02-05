#include <EnginePch.h>
#include <World/Tile.h>

Tile::Tile(uint32 material, const glm::vec3& pos, uint32 id) : GameObject()
{
	//m_Color = color;
	GameObject::SetMesh(MESH::QUAD);
	GameObject::SetMaterial(material);
	GameObject::SetRotation(glm::vec4(1.0f, 0.0f, 0.0f, -glm::pi<float>() / 2.0f));
	GameObject::SetPosition(pos);
	GameObject::UpdateTransform();
}

Tile::~Tile()
{
	
}

void Tile::SetID(uint32 id)
{
	m_Id = id;
}

void Tile::SetColor(const glm::vec4& color)
{
	//m_pMaterial->SetColor(color);
	m_Color = color;
}

void Tile::SetTint(const glm::vec4& tint)
{
	glm::vec4 finalColor = (m_Color + tint) / 2.0f;
	//m_pMaterial->SetColor(finalColor);
}

void Tile::ResetColor()
{
	//m_pMaterial->SetColor(m_Color);
}
