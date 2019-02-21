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
	m_IsReflectable(false),
	m_IsVisible(true),
	m_TypeId(-1)
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

void GameObject::SetIsVisible(bool isVisible) noexcept
{
	m_IsVisible = isVisible;
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

void GameObject::SetRoom(uint32 room) noexcept
{
	m_Room = room;
}

void GameObject::SetHidden(bool isHidden) noexcept
{
	m_IsHidden = isHidden;
}

void GameObject::UpdateTransform() noexcept
{
	if (m_IsDirty && m_IsVisible)
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

void GameObject::Update(float deltaTime) noexcept
{
	UpdateTransform();
}
