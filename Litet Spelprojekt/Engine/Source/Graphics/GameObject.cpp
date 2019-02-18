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
	if (!m_Extending)
	{
		m_OriginalPos = position;
		m_OriginalPos.x -= 5 * floor(glm::clamp(m_Position.y, 0.0f, 4.0f) / 2.0f) * 2.0f * m_Extended;
	}
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

void GameObject::SetExtend(bool extend) noexcept
{
	if (extend)
	{
		m_Extending = true;
		m_Extended = true;
		m_ExtendPosX = m_OriginalPos.x + 5 * floor(glm::clamp(m_Position.y, 0.0f, 4.0f) / 2.0f) * 2.0f;
	}
	else if (!extend)
	{
		m_Extending = true;
		m_Extended = false;
		m_ExtendPosX = m_OriginalPos.x;
	}
}

void GameObject::Extend(float dtS) noexcept
{
	if (m_Extending)
	{
		if (std::abs(m_Position.x - m_ExtendPosX) > 0.01)
		{
			float move = m_ExtendPosX - m_Position.x;
			float extensionSpeed = 20.0f;
			move /= std::abs(move);
			move *= floor(glm::clamp(m_Position.y, 0.0f, 4.0f) / 2.0f) * 2.0f * extensionSpeed;
			SetPosition(glm::vec3(m_Position.x + move * dtS, m_Position.y, m_Position.z));
		}
		else
		{
			m_Extending = false;
			m_OriginalPos = m_Position;
			m_OriginalPos.x -= 5 * floor(glm::clamp(m_Position.y, 0.0f, 4.0f) / 2.0f) * 2.0f * m_Extended;
		}
	}
}

void GameObject::SetIsCrew(bool isCrew) noexcept
{
	m_IsCrew = isCrew;
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

void GameObject::Update(float deltaTime)
{
	Extend(deltaTime);
	UpdateTransform();
	//////
}
