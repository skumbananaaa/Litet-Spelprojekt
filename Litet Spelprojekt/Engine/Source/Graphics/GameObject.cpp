#include <EnginePch.h>
#include <Graphics\GameObject.h>

GameObject::GameObject() noexcept
	: m_pMesh(nullptr),
	m_pDecal(nullptr),
	m_pMaterial(nullptr),
	m_Position(0.0f),
	m_Rotation(1.0f, 0.0f, 0.0f, 0.0f),
	m_Scale(1.0f),
	m_IsDirty(true),
	m_IsReflectable(false)
{
	UpdateTransform();
}

GameObject::~GameObject()
{
}

void GameObject::SetName(const std::string& name) noexcept
{
	m_Name = name;
}

void GameObject::SetIsReflectable(bool isReflectable) noexcept
{
	m_IsReflectable = isReflectable;
}

void GameObject::SetPosition(const glm::vec3& position) noexcept
{
	m_Position = position;
	m_IsDirty = true;
}

void GameObject::SetRotation(const glm::vec4& rotation) noexcept
{
	m_Rotation = rotation;
	m_IsDirty = true;
}

void GameObject::SetScale(const glm::vec3& scale) noexcept
{
	m_Scale = scale;
	m_IsDirty = true;
}

void GameObject::UpdateTransform() noexcept
{
	if (m_IsDirty)
	{
		glm::mat4 transform(1.0f);
		transform = glm::translate(transform, m_Position);
		transform = glm::rotate(transform, m_Rotation.w, glm::vec3(m_Rotation.x, m_Rotation.y, m_Rotation.z));
		transform = glm::scale(transform, m_Scale);
		m_transform = transform;
		m_InverseTransform = glm::inverse(m_transform);
		
		m_IsDirty = false;
	}
}

void GameObject::Update(float deltaTime)
{
	UpdateTransform();
	//////
}
