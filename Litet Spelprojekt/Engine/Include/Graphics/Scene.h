#pragma once

#include <EnginePch.h>
#include <Graphics\GameObject.h>
#include <Graphics\Camera.h>

class API Scene
{
public:
	Scene() noexcept;
	~Scene();

	void SetCamera(Camera* pCamera) noexcept;
	void AddGameObject(GameObject* pGameObject) noexcept;
	void RemoveGameObject(uint32 index) noexcept;

	Camera& GetCamera() const noexcept;
	const std::vector<GameObject*>& GetGameObjects() const noexcept;

	void OnUpdate(float dtS) noexcept;

private:
	Camera* m_pCamera;
	std::vector<GameObject*> m_GameObjects;
};

inline Camera& Scene::GetCamera() const noexcept
{
	return *m_pCamera;
}

inline const std::vector<GameObject*>& Scene::GetGameObjects() const noexcept
{
	return m_GameObjects;
}