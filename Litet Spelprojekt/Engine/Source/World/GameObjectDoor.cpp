#include <EnginePch.h>
#include <World\GameObjectDoor.h>
#include <Graphics/Scene.h>

GameObjectDoor::GameObjectDoor() noexcept
{
	SetMaterial(MATERIAL::WHITE);
	SetMesh(MESH::DOOR);
	UpdateTransform();

	m_Percentage = 1.0f;
	m_Open = true;

}

GameObjectDoor::~GameObjectDoor()
{

}

void GameObjectDoor::SetOpen(bool open) noexcept
{
	m_Open = open;
}

bool GameObjectDoor::IsOpen() const noexcept
{
	return m_Percentage >= 1.0F;
}

bool GameObjectDoor::IsClosed() const noexcept
{
	return m_Percentage <= 0.0F;
}

void GameObjectDoor::Update(const Camera& camera, float deltaTime) noexcept
{
	GameObject::Update(camera, deltaTime);

	//Test
	/*if (IsOpen())
	{
		SetOpen(false);
	}
	else if(IsClosed())
	{
		SetOpen(true);
	}*/


	const float lastPercentage = m_Percentage;
	m_Percentage += deltaTime * (m_Open * 2 - 1);
	m_Percentage = glm::clamp<float>(m_Percentage, 0.0F, 1.0F);
	if (lastPercentage != m_Percentage)
	{
		m_IsDirty = true;
		UpdateTransform();
	}
}

int32 GameObjectDoor::TestAgainstRay(const glm::vec3 ray, const glm::vec3 origin, float extension) noexcept
{
	glm::vec3 centre = GetPosition() + glm::vec3(0.0f, 1.0f, 0.0f);
	centre.x += extension * glm::floor(centre.y / 2.0f);

	glm::vec3 normals[]{
		m_Direction,
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::normalize(glm::cross(normals[0], normals[1]))
	};

	float h[] = {
		0.15,
		1.0,
		0.5
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

void GameObjectDoor::UpdateTransform() noexcept
{
	if (m_IsDirty && m_IsVisible)
	{
		const glm::vec4& rotation = GetRotation();
		glm::mat4 transform(1.0f);

		transform = glm::translate(transform, GetPosition()); // 5. Translate to world position
		transform = glm::rotate(transform, rotation.w, glm::vec3(rotation.x, rotation.y, rotation.z)); // 4. Rotate To Align with the walls
		transform = glm::translate(transform, glm::vec3(0.0F, 0.0F, 0.5F)); // 3. Translate To Origin
		transform = glm::rotate(transform, m_Percentage * glm::half_pi<float>(), glm::vec3(0.0F, 1.0f, 0.0F)); // 2. Rotate along the hinge
		transform = glm::translate(transform, glm::vec3(0.0F, 0.0F, -0.5F)); // 1. Translate To The Edge

		transform = glm::scale(transform, GetScale());
		m_transform = transform;
		m_InverseTransform = glm::inverse(m_transform);

		m_IsDirty = false;
	}
}

void GameObjectDoor::OnPicked() noexcept
{
	std::cout << "i am a picked door!" << std::endl;
}

void GameObjectDoor::OnAddedToScene(Scene* scene) noexcept
{
	scene->RegisterPickableGameObject(this);
}
