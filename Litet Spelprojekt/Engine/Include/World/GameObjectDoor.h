#pragma once
#include <EnginePch.h>
#include <Graphics/GameObject.h>

class API GameObjectDoor : public GameObject
{
public:
	GameObjectDoor() noexcept;
	virtual ~GameObjectDoor();

	void SetOpen(bool open) noexcept;
	bool IsOpen() const noexcept;
	bool IsClosed() const noexcept;

	virtual void Update(float deltaTime) noexcept override;
	virtual void UpdateTransform() noexcept override;

private:
	float m_Percentage;
	bool m_Open;
};