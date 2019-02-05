#include <EnginePch.h>
#include <Graphics\Scene.h>

Scene::Scene() noexcept
{
	m_pCamera = nullptr;
	m_pSkyBox = nullptr;
}

Scene::~Scene()
{
	for (size_t i = 0; i < m_Cameras.size(); i++)
	{
		DeleteSafe(m_Cameras[i]);
	}

	DeleteSafe(m_pSkyBox);

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

	for (size_t i = 0; i < m_SpotLights.size(); i++)
	{
		DeleteSafe(m_SpotLights[i]);
	}
}

void Scene::SetCamera(Camera* pCamera, uint32 index) noexcept
{
	if (m_Cameras.size() == index)
	{
		m_Cameras.push_back(pCamera);
	}
	else if (index > m_Cameras.size())
	{
		std::cout << "Failed to set camera!!! Index out of range" << std::endl;
		return;
	}
	else
	{
		m_Cameras[index] = pCamera;
	}
	m_pCamera = pCamera;
}

void Scene::SelectCamera(uint32 index)
{
	m_pCamera = m_Cameras[index];
}

void Scene::SetSkyBox(SkyBox * pSkyBox) noexcept
{
	m_pSkyBox = pSkyBox;
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
