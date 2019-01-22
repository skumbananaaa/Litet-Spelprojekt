#include <EnginePch.h>
#include <Graphics\Scene.h>

Scene::Scene() noexcept
{
	m_pCamera = nullptr;
}

Scene::~Scene()
{
	if (m_pCamera != nullptr)
	{
		delete m_pCamera;
	}

	for (uint32 i = 0; i < m_GameObjects.size(); i++)
	{
		delete m_GameObjects[i];
		m_GameObjects[i] = nullptr;
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

void Scene::RemoveGameObject(uint32 index) noexcept
{
	m_GameObjects.erase(m_GameObjects.begin() + index);
}

void Scene::OnUpdate(float dtS) noexcept
{
	m_pCamera->Update();
	
	for (uint32 i = 0; i < m_GameObjects.size(); i++)
	{
		m_GameObjects[i]->UpdateTransform();
	}
}
