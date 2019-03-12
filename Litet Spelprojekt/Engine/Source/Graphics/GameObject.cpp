#include <EnginePch.h>
#include <Graphics\GameObject.h>
#include <World/World.h>

GameObject::GameObject() noexcept
	: m_pMesh(nullptr),
	m_pAMesh(nullptr),
	m_pASkeleton(nullptr),
	m_pDecal(nullptr),
	m_pMaterial(nullptr),
	m_Position(0.0f),
	m_Rotation(1.0f, 0.0f, 0.0f, 0.0f),
	m_Scale(1.0f),
	m_IsDirty(true),
	m_IsVisible(true),
	m_IsHovered(false),
	m_IsPicked(false),
	m_IsTickable(false),
	m_TypeId(-1),
	m_Room(0)
{
	SetDirection(glm::vec3(-1.0f, 0.0f, 0.0f));
	UpdateTransform();
}

GameObject::~GameObject()
{
	DeleteSafe(m_pASkeleton);
}

void GameObject::SetName(const std::string& name) noexcept
{
	m_Name = name;
}

void GameObject::SetIsVisible(bool isVisible) noexcept
{
	m_IsVisible = isVisible;
}

void GameObject::SetIsPicked(bool picked) noexcept
{
	m_IsPicked = picked;
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

void GameObject::SetDirection(const glm::vec3& direction) noexcept
{
	m_Direction = glm::normalize(direction);
}

void GameObject::OnHovered() noexcept
{
	//m_IsHovered = true;
	//Game::GetGame()->m_pSceneGame->GetUICrewMember()->SetCrewMember(this);
}

void GameObject::OnNotHovered() noexcept
{
	//m_IsHovered = false;
	//Game::GetGame()->m_pSceneGame->GetUICrewMember()->SetCrewMember(nullptr);
}
void GameObject::OnPicked(const std::vector<int32>& selectedMembers, int32 x, int32 y) noexcept
{
	//m_IsPicked = true;
}

int32 GameObject::TestAgainstRay(const glm::vec3 ray, const glm::vec3 origin, float extension) noexcept
{
	return true;
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

		if (HasSkeleton())
		{
			m_pASkeleton->SetSkeletonTransform(m_transform);
		}
	}
}

void GameObject::UpdateParallel(float dtS) noexcept
{
	if (IsVisible())
	{
		//if (m_pWorld->GetRoom(GetRoom()).IsActive() || !IsHidden())
		{
			Lock();
			const AnimatedSkeleton& skeleton = *GetSkeleton();
			skeleton.UpdateBoneTransforms(dtS, GetAnimatedMesh());
			Unlock();
		}
	}
}

void GameObject::Update(const Camera& camera, float deltaTime) noexcept
{
}