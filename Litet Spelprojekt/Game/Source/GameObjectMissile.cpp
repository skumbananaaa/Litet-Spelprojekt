#include "..\Include\GameObjectMissile.h"
#include <Graphics/Scene.h>

GameObjectMissile::GameObjectMissile(const glm::vec3& pos, const glm::vec3& target) noexcept :
	m_DistanceTraveled(0),
	m_pMeshEmitter(nullptr)
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

GameObjectMissile::~GameObjectMissile()
{

}

void GameObjectMissile::Update(const Camera& camera, float deltaTime) noexcept
{
	float step = m_Velocity * deltaTime;
	m_DistanceTraveled += step;
	SetPosition(GetPosition() + (step * m_Direction));
	m_pMeshEmitter->SetPosition(GetPosition());
	m_pMeshEmitter->UpdateTransform();
	UpdateTransform();
}

void GameObjectMissile::UpdateTransform() noexcept
{
	GameObject::UpdateTransform();
}

bool GameObjectMissile::HasHitTarget() const noexcept
{
	return m_DistanceTraveled >= m_TotalDistance;
}

void GameObjectMissile::OnAddedToScene(Scene* scene) noexcept
{
	m_pMeshEmitter = new MeshEmitter();
	m_pMeshEmitter->SetMesh(MESH::MESH_PARTICLE);
	m_pMeshEmitter->SetTimeToLive(1.0f);
	m_pMeshEmitter->SetConeAngle(glm::radians<float>(100.0f));
	m_pMeshEmitter->SetSpeed(2.0f, 4.0f);
	m_pMeshEmitter->SetScale(glm::vec2(0.1f), glm::vec2(0.5f));
	m_pMeshEmitter->SetBeginColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	m_pMeshEmitter->AddColorNode(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), 0.2F);
	m_pMeshEmitter->AddColorNode(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f), 0.3F);
	m_pMeshEmitter->SetEndColor(glm::vec4(0.05f, 0.05f, 0.05f, 1.0f));
	m_pMeshEmitter->SetPosition(GetPosition());
	m_pMeshEmitter->SetRotation(GetRotation());
	m_pMeshEmitter->SetParticlesPerSeconds(80);
	m_pMeshEmitter->UpdateTransform();
	m_pMeshEmitter->SetIsVisible(true);
	scene->AddGameObject(m_pMeshEmitter);
}

void GameObjectMissile::OnRemovedFromScene(Scene* scene) noexcept
{
	scene->RemoveGameObject(m_pMeshEmitter);
	DeleteSafe(m_pMeshEmitter);
}