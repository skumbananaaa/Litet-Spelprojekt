#pragma once
#include <EnginePch.h>
#include <Graphics/GameObject.h>
#include "../Include/GUI/UIOrder.h"
#include <World/World.h>

class GameObjectDoor : public GameObject, public UIOrder
{
public:
	GameObjectDoor() noexcept;
	virtual ~GameObjectDoor();

	void SetOpen(bool open) noexcept;
	bool IsOpen() const noexcept;
	bool IsClosed() const noexcept;

	DOOR_COLOR GetColor() const noexcept;

	virtual void Update(const Camera& camera, float deltaTime) noexcept override;
	virtual void UpdateTransform() noexcept override;
	int32 TestAgainstRay(const glm::vec3 ray, const glm::vec3 origin, float extension) noexcept override;

	void OnPicked(const std::vector<int32>& selectedMembers, int32 x, int32 y) noexcept override;

	virtual void OnAddedToScene(Scene* scene) noexcept override;

	virtual void OnOrderChosen(const std::string& name, void* userData, const std::vector<int32>& selectedMembers) noexcept override;

private:
	float m_Percentage;
	bool m_Open;
};