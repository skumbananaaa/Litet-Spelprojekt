#include <EnginePch.h>
#include <World\GameObjectDoor.h>

GameObjectDoor::GameObjectDoor() noexcept
{
	SetMaterial(MATERIAL::WHITE);
	SetMesh(MESH::DOOR);
	UpdateTransform();
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

void GameObjectDoor::Update(float deltaTime) noexcept
{
	GameObject::Update(deltaTime);

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
	m_Percentage -= deltaTime * (m_Open * 2 - 1);
	m_Percentage = glm::clamp<float>(m_Percentage, 0.0F, 1.0F);
	if (lastPercentage != m_Percentage)
	{
		m_IsDirty = true;
	}
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
