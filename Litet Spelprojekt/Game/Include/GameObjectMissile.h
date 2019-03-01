#pragma once
#include <Graphics/GameObject.h>
#include <Graphics/Particles/MeshEmitter.h>

class GameObjectMissile : public GameObject
{
public:
	GameObjectMissile(const glm::vec3& pos, const glm::vec3& target) noexcept;
	virtual ~GameObjectMissile();

	virtual void Update(const Camera& camera, float deltaTime) noexcept override;
	virtual void UpdateTransform() noexcept override;

	bool HasHitTarget() const noexcept;

	virtual void OnAddedToScene(Scene* scene) noexcept override;
	virtual void OnRemovedFromScene(Scene* scene) noexcept override;

private:
	MeshEmitter* m_pMeshEmitter;
	glm::vec3 m_Direction;
	float m_Velocity;
	float m_TotalDistance;
	float m_DistanceTraveled;
};