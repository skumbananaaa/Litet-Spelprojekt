#pragma once
#include <Graphics/GameObject.h>
#include "../Include/Game.h"
#include "GUI/UIOrder.h"

class GameObjectFloor : public GameObject, public UIOrder
{
public:
	GameObjectFloor();
	~GameObjectFloor();

	int32 TestAgainstRay(const glm::vec3 ray, const glm::vec3 origin, float elevation, float extension) noexcept override;

	void OnPicked(const std::vector<int32>& selectedMembers, int32 x, int32 y) noexcept override;

	virtual void OnAddedToScene(Scene* scene) noexcept override;

	virtual void OnOrderChosen(const std::string& name, void* userData, const std::vector<int32>& selectedMembers) noexcept override;

private:
	glm::ivec3 FindClosestExtinguisher(const glm::vec3& currentPosition) noexcept;
	std::vector<GameObject*> m_pExtinguishers;
};


inline glm::ivec3 GameObjectFloor::FindClosestExtinguisher(const glm::vec3& currentPosition) noexcept
{
	GameObject* pGameObject = nullptr;
	float shortDistance = FLT_MAX;
	int index = -1;

	for (uint32 i = 0; (pGameObject = Game::GetGame()->m_pSceneGame->GetGameObject("Extinguisher" + std::to_string(i))) != nullptr; i++)
	{
		m_pExtinguishers.push_back(pGameObject);
	}


	for (int i = 0; i < m_pExtinguishers.size(); i++)
	{
		float distance = glm::distance(m_pExtinguishers[i]->GetPosition(), currentPosition);
		if (distance < shortDistance)
		{
			index = i;
			shortDistance = distance;
		}
	}


	return m_pExtinguishers[index]->GetTile();
}
