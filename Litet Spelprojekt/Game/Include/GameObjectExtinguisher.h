#pragma once
#include <EnginePch.h>
#include <Graphics/GameObject.h>
#include <World/World.h>
#include "GUI/UIOrder.h"

class GameObjectExtinguisher : public GameObject, public UIOrder
{
public:
	GameObjectExtinguisher() noexcept;
	virtual ~GameObjectExtinguisher();

	bool AccessRequest(uint32 shipNr) noexcept;
	bool RemoveFromQueue(uint32 shipNr) noexcept;


	virtual void Update(const Camera& camera, float deltaTime) noexcept override;
	virtual void UpdateTransform() noexcept override;
	int32 TestAgainstRay(const glm::vec3 ray, const glm::vec3 origin, float elevation, float extension) noexcept override;

	void OnPicked(const std::vector<int32>& selectedMembers, int32 x, int32 y) noexcept override;

	virtual void OnAddedToScene(Scene* scene) noexcept override;
	virtual void OnOrderChosen(const std::string& name, void* userData, const std::vector<int32>& selectedMembers) noexcept override;
	virtual void SetPosition(const glm::vec3& position) noexcept override;

private:
	std::vector<uint32> m_AccessQueue;
};