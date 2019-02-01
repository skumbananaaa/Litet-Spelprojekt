#pragma once

#include <EnginePch.h>
#include <Graphics/GameObject.h>
#include <Graphics/Camera.h>
#include <Graphics/Lights/DirectionalLight.h>
#include <Graphics/Lights/PointLight.h>
#include <Graphics/Lights/SpotLight.h>

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
	void AddSpotLight(SpotLight* pLight) noexcept;
	void RemoveGameObject(uint32 index) noexcept;

	Camera& GetCamera() noexcept;
	const Camera& GetCamera() const noexcept;

	const std::vector<GameObject*>& GetGameObjects() const noexcept;
	const std::vector<GameObject*>& GetDrawables() const noexcept;
	const std::vector<GameObject*>& GetDecals() const noexcept;
	const std::vector<GameObject*>& GetReflectables() const noexcept;
	const std::vector<DirectionalLight*>& GetDirectionalLights() const noexcept;
	const std::vector<PointLight*>& GetPointLights() const noexcept;
	const std::vector<SpotLight*>& GetSpotLights() const noexcept;

private:
	Camera* m_pCamera;
	std::vector<GameObject*> m_GameObjects;
	std::vector<GameObject*> m_Drawables;
	std::vector<GameObject*> m_Decals;
	std::vector<GameObject*> m_Reflectables;
	std::vector<DirectionalLight*> m_DirectionalLights;
	std::vector<PointLight* > m_PointLights;
	std::vector<SpotLight* > m_SpotLights;
};

inline Camera& Scene::GetCamera() noexcept
{
	assert(m_pCamera != nullptr);
	return *m_pCamera;
}

inline const Camera& Scene::GetCamera() const noexcept
{
	assert(m_pCamera != nullptr);
	return *m_pCamera;
}

inline const std::vector<GameObject*>& Scene::GetGameObjects() const noexcept
{
	return m_GameObjects;
}

inline const std::vector<GameObject*>& Scene::GetDrawables() const noexcept
{
	return m_Drawables;
}

inline const std::vector<GameObject*>& Scene::GetDecals() const noexcept
{
	return m_Decals;
}

inline const std::vector<GameObject*>& Scene::GetReflectables() const noexcept
{
	return m_Reflectables;
}

inline const std::vector<DirectionalLight*>& Scene::GetDirectionalLights() const noexcept
{
	return m_DirectionalLights;
}

inline const std::vector<PointLight*>& Scene::GetPointLights() const noexcept
{
	return m_PointLights;
}

inline const std::vector<SpotLight*>& Scene::GetSpotLights() const noexcept
{
	return m_SpotLights;
}