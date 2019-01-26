#pragma once

#include <EnginePch.h>
#include <Graphics/GameObject.h>
#include <Graphics/Camera.h>
#include <Graphics/Lights/DirectionalLight.h>
#include <Graphics/Lights/PointLight.h>

class API Scene
{
public:
	Scene() noexcept;
	~Scene();

	void OnUpdate(float dtS) noexcept;
	
	void SetCamera(Camera* pCamera) noexcept;
	void AddGameObject(GameObject* pGameObject) noexcept;
	void AddDirectionalLight(DirectionalLight* pLight) noexcept;
	void AddPointLight(PointLight* pLight) noexcept;
	void RemoveGameObject(uint32 index) noexcept;

	Camera& GetCamera() const noexcept;
	const std::vector<GameObject*>& GetGameObjects() const noexcept;
	const std::vector<DirectionalLight*>& GetDirectionalLights() const noexcept;
	const std::vector<PointLight*>& GetPointLights() const noexcept;

private:
	Camera* m_pCamera;
	std::vector<GameObject*> m_GameObjects;
	std::vector<DirectionalLight*> m_DirectionalLights;
	std::vector<PointLight*> m_PointLights;
};

inline Camera& Scene::GetCamera() const noexcept
{
	return *m_pCamera;
}

inline const std::vector<GameObject*>& Scene::GetGameObjects() const noexcept
{
	return m_GameObjects;
}

inline const std::vector<DirectionalLight*>& Scene::GetDirectionalLights() const noexcept
{
	return m_DirectionalLights;
}

inline const std::vector<PointLight*>& Scene::GetPointLights() const noexcept
{
	return m_PointLights;
}
