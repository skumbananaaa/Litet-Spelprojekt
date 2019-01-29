#include <EnginePch.h>
#include <Graphics\Scene.h>

Scene::Scene() noexcept
{
	m_pCamera = nullptr;
}

Scene::~Scene()
{
	DeleteSafe(m_pCamera);

	for (size_t i = 0; i < m_GameObjects.size(); i++)
	{
		DeleteSafe(m_GameObjects[i]);
	}

	for (size_t i = 0; i < m_DirectionalLights.size(); i++)
	{
		DeleteSafe(m_DirectionalLights[i]);
	}

	for (size_t i = 0; i < m_PointLights.size(); i++)
	{
		DeleteSafe(m_PointLights[i]);
	}
}

void Scene::SetCamera(Camera* pCamera) noexcept
{
	m_pCamera = pCamera;
}

void Scene::AddGameObject(GameObject* pGameObject) noexcept
{
	m_GameObjects.push_back(pGameObject);
}

void Scene::AddDirectionalLight(DirectionalLight* pLight) noexcept
{
	m_DirectionalLights.push_back(pLight);
}

void Scene::AddPointLight(PointLight* pLight) noexcept
{
	m_PointLights.push_back(pLight);
}

void Scene::AddSpotLight(SpotLight* pLight) noexcept
{
	m_SpotLights.push_back(pLight);
}

void Scene::RemoveGameObject(uint32 index) noexcept
{
	m_GameObjects.erase(m_GameObjects.begin() + index);
}

void Scene::OnUpdate(float dtS) noexcept
{
	for (uint32 i = 0; i < m_GameObjects.size(); i++)
	{
		m_GameObjects[i]->UpdateTransform();
	}
}
