#include <EnginePch.h>
#include <Graphics\GameObject.h>

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
	m_TypeId(-1)
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
void GameObject::OnPicked() noexcept
{
	//m_IsPicked = true;
}

int32 GameObject::TestAgainstRay(const glm::vec3 ray, const glm::vec3 origin, float extension) noexcept
{
	glm::vec3 centre = GetPosition() + glm::vec3(0.0f, 0.9f, 0.0f);
	centre.x += extension * glm::floor(centre.y / 2.0f);

	glm::vec3 normals[]{
		m_Direction,
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::normalize(glm::cross(normals[0], normals[1]))
	};

	float h[] = {
		0.1,
		0.9,
		0.25
	};

	float d1[] = {
		glm::dot(centre - normals[0] * h[0], normals[0]),
		glm::dot(centre - normals[1] * h[1], normals[1]),
		glm::dot(centre - normals[2] * h[2], normals[2])
	};
	float d2[] = {
		glm::dot(centre + normals[0] * h[0], normals[0]),
		glm::dot(centre + normals[1] * h[1], normals[1]),
		glm::dot(centre + normals[2] * h[2], normals[2])
	};

	float t1[3];
	float t2[3];
	float t_min[3];
	float t_max[3];

	float t = -1;
	float min_t, max_t;

	for (int j = 0; j < 3; j++)
	{
		if (std::abs(glm::dot(normals[j], ray)) > 0.01)
		{
			t1[j] = (d1[j] - glm::dot(normals[j], origin)) / glm::dot(normals[j], ray);
			t2[j] = (d2[j] - glm::dot(normals[j], origin)) / glm::dot(normals[j], ray);

			t_min[j] = std::min(t1[j], t2[j]);
			t_max[j] = std::max(t1[j], t2[j]);
		}
		else if (-glm::dot(normals[0], centre - origin) - h[j] > 0 || -glm::dot(normals[0], centre - origin) + h[j] < 0)
			return -1;
	}

	min_t = std::max(t_min[0], t_min[1]);
	min_t = std::max(min_t, t_min[2]);
	max_t = std::min(t_max[0], t_max[1]);
	max_t = std::min(max_t, t_max[2]);

	if (min_t <= max_t && max_t >= 0)
	{
		if (t_min > 0)
			t = min_t;
		else
			t = max_t;
	}

	return t;
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

void GameObject::Update(const Camera& camera, float deltaTime) noexcept
{
}
