#include <EnginePch.h>
#include <Graphics\GameObject.h>

GameObject::GameObject() noexcept
	: m_pMesh(nullptr),
	m_pMaterial(nullptr),
	m_position(0.0f),
	m_rotation(1.0f, 0.0f, 0.0f, 0.0f),
	m_scale(1.0f),
	m_IsDirty(true)
{
	UpdateTransform();
}

GameObject::~GameObject()
{
}

void GameObject::SetMesh(const IndexedMesh* const mesh) noexcept
{
	m_pMesh = mesh;
}

void GameObject::SetPosition(const glm::vec3& position) noexcept
{
	m_position = position;
	m_IsDirty = true;
}

void GameObject::SetRotation(const glm::vec4& rotation) noexcept
{
	m_rotation = rotation;
	m_IsDirty = true;
}

void GameObject::SetScale(const glm::vec3& scale) noexcept
{
	m_scale = scale;
	m_IsDirty = true;
}

void GameObject::UpdateTransform() noexcept
{
	if (m_IsDirty)
	{
		glm::mat4 transform(1.0f);
		transform = glm::translate(transform, m_position);
		transform = glm::rotate(transform, m_rotation.w, glm::vec3(m_rotation.x, m_rotation.y, m_rotation.z));
		transform = glm::scale(transform, m_scale);
		m_transform = transform;
		m_IsDirty = false;
	}
}
