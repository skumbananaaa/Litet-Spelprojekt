#include <EnginePch.h>
#include <World/Tile.h>

Tile::Tile(uint32 material, const glm::vec3& pos, uint32 id) : GameObject()
{
	m_HasStairs = false;
	m_DefaultMaterial = material;
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

void Tile::SetDefaultMaterial(uint32 material) noexcept
{
	m_DefaultMaterial = material;
	GameObject::SetMaterial(material);
}

void Tile::ResetMaterial() noexcept
{
	GameObject::SetMaterial(m_DefaultMaterial);
}

void Tile::SetHasStairs(bool value) noexcept
{
	m_HasStairs = value;
}
