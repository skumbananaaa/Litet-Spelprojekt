#pragma once
#include <Graphics/GameObject.h>

class GameObjectMissile : public GameObject
{
public:
	GameObjectMissile() noexcept;
	virtual ~GameObjectMissile();

	virtual void Update(const Camera& camera, float deltaTime) noexcept override;
	virtual void UpdateTransform() noexcept override;

private:
	float m_Velocity;
};