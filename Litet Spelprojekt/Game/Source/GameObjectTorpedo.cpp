#include "..\Include\GameObjectTorpedo.h"
#include <Graphics/Scene.h>

GameObjectTorpedo::GameObjectTorpedo(const glm::vec3& pos, const glm::vec3& target) noexcept :
	m_DistanceTraveled(0)
{
	SetMaterial(MATERIAL::WHITE);
	SetMesh(MESH::MISSILE);

	SetPosition(pos);

	glm::vec3 delta = target - pos;
	m_TotalDistance = glm::length(delta);
	m_Direction = glm::normalize(delta);
	m_Velocity = 10;

	glm::vec3 forward(0.0F, 1.0F, 0.0F);
	float dot = glm::dot(forward, m_Direction);

	if (glm::abs(dot - (-1.0f)) < 0.000001f)
	{
		SetRotation(glm::vec4(1.0F, 0.0F, 0.0F, glm::pi<float>()));
	}
	else if (glm::abs(dot - (1.0f)) < 0.000001f)
	{

	}
	else
	{
		float rotAngle = (float)glm::acos(dot);
		glm::vec3 rotAxis = glm::cross(forward, m_Direction);
		rotAxis = glm::normalize(rotAxis);
		SetRotation(glm::vec4(rotAxis.x, rotAxis.y, rotAxis.z, rotAngle));
	}


	UpdateTransform();
}

GameObjectTorpedo::~GameObjectTorpedo()
{

}

void GameObjectTorpedo::Update(const Camera& camera, float deltaTime) noexcept
{
	float step = m_Velocity * deltaTime;
	m_DistanceTraveled += step;
	SetPosition(GetPosition() + (step * m_Direction));
	UpdateTransform();
}

void GameObjectTorpedo::UpdateTransform() noexcept
{
	GameObject::UpdateTransform();
}

bool GameObjectTorpedo::HasHitTarget() const noexcept
{
	return m_DistanceTraveled >= m_TotalDistance;
}