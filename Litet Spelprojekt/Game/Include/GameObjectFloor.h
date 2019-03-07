#pragma once
#include <Graphics/GameObject.h>
#include "GUI/UIOrder.h"

class GameObjectFloor : public GameObject, public UIOrder
{
public:
	GameObjectFloor();
	~GameObjectFloor();

	int32 TestAgainstRay(const glm::vec3 ray, const glm::vec3 origin, float extension) noexcept override;

	void OnPicked(const std::vector<int32>& selectedMembers, int32 x, int32 y) noexcept override;

	virtual void OnAddedToScene(Scene* scene) noexcept override;

	virtual void OnOrderChosen(const std::string& name, void* userData, const std::vector<int32>& selectedMembers) noexcept override;
};