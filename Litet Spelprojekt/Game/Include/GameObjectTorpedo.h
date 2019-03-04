#pragma once
#include <Graphics/GameObject.h>
#include <Graphics/Particles/MeshEmitter.h>

class GameObjectTorpedo : public GameObject
{
public:
	GameObjectTorpedo(const glm::vec3& pos, const glm::vec3& target) noexcept;
	virtual ~GameObjectTorpedo();

	virtual void Update(const Camera& camera, float deltaTime) noexcept override;
	virtual void UpdateTransform() noexcept override;

	bool HasHitTarget() const noexcept;

private:
	glm::vec3 m_Direction;
	float m_Velocity;
	float m_TotalDistance;
	float m_DistanceTraveled;
};